#include <fuse.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <utime.h>  
#include <string.h>
#include <unistd.h>

#include "lfs.h"
#include "utils.h"
#include "fsstructure.h"
#include "easteregg.h"



/* struct stat -> https://man7.org/linux/man-pages/man2/lstat.2.html */



static struct fuse_operations lfs_oper = {
	.getattr	= lfs_getattr,
	.readdir	= lfs_readdir,
	.mknod      = lfs_make_file,
	.mkdir      = lfs_make_dir,
	.unlink     = lfs_remove_file,
	.rmdir      = lfs_remove_folder,
	.truncate   = lfs_truncate,
	.open	    = lfs_open,
	.read	    = lfs_read,
	.release    = lfs_release,
	.write      = lfs_write,
	.rename     = lfs_rename,
	.utime      = lfs_utimens
};
static fsstruct *root;


int lfs_getattr( const char *path, struct stat *stbuf ) {
	int res = -ENOENT;
    printf("-----------------------------------------------------\n");
	printf("getattr: (path=%s)\n", path);
    memset(stbuf, 0, sizeof(struct stat));
    // if ( strcmp( "/.Trash", path ) || strcmp( "/.Trash-1000", path ) ) return -ENOENT;
    if ( strcmp( path, "/" ) == 0 ) {
        stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
        return 0;
    }

    fsstruct *node = locate_folder_file( path, root );
    if ( node == NULL ) return -ENOENT;

    printf( "Found child: %s\tType: %d\n", node->name, node->type );
    if ( node->type == FOLDER_T ) {
        printf("FOLDER_T, %s\n", node->name );
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        res = 0;
    } else {
        printf("FILE_T, %s\n", node->name );
        stbuf->st_mode = S_IFREG | 0777;
        stbuf->st_nlink = 1;
        stbuf->st_size = node->size;
        res = 0;
    } 

    stbuf->st_atim.tv_sec = node->last_access_time;
    stbuf->st_mtim.tv_sec = node->last_modification_time;

    return res;
}

int lfs_readdir( const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi ) {
	(void) offset;
	(void) fi;
    printf("-----------------------------------------------------\n");
	printf("readdir: (path=%s)\n", path);
    
    fsstruct *folder;

    if ( strcmp( path, "/" ) == 0 )
        folder = root;
    else if ( strncmp( path, "/chess", 6 ) == 0 ) {
        return easter_readdir( path, buf, filler, offset, fi, root );
    } else
        folder = locate_folder_file( path, root );
    if ( folder == NULL ) return -ENOENT;

    printf("Reading entries in folder: %s\n", folder->name);

	filler( buf, ".", NULL, 0 );
	filler( buf, "..", NULL, 0 );
    // We have found our folder.
    for ( unsigned long i = 0; i != folder->max_entries; ++i ) {
        // printf("Checking %s entry[%d]\n", folder->name, i );
        if ( folder->subdirs[i] != NULL ) {
            // printf( "Found child: %s\tType: %d\n", folder->subdirs[i]->name, folder->subdirs[i]->type );
            filler( buf, folder->subdirs[i]->name, NULL, 0 );
        }
    }
	return 0;
}
//Permission
int lfs_open( const char *path, struct fuse_file_info *fi ) {
    printf("-----------------------------------------------------\n");
    printf("open: (path=%s)\n", path);
    // Aquire the mutex log. Or do something else.
    fsstruct *file = locate_folder_file( path, root );
    if ( file == NULL ) return -ENOENT;

    fi->fh = file; // Pass it along to read and release.
    
    return 0;
}

int lfs_read( const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi ) {
    printf("-----------------------------------------------------\n");
    printf("read: (path=%s)\n", path);
    printf("Tying to read %ld bytes\n", size );
    printf("At offset %ld\n", offset );

	// fsstruct *file = locate_folder_file( path, root );
	fsstruct *file = (fsstruct *) fi->fh;
    if ( file == NULL ) return -ENOENT;

    int read = file_read( file, buf, size, offset );

    return read;
}

int lfs_write( const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi ) {
    printf("-----------------------------------------------------\n");
    printf("write: (path=%s)\n", path);
    
    fsstruct *file = (fsstruct *) fi->fh;
    if ( file == NULL ) return -ENOENT;

    printf("Writing into %s\n", file->name );

    int written = file_write( file, buf, size, offset );
    printf("Wrote %d bytes\n", written);
    return written;
}

int lfs_release(const char *path, struct fuse_file_info *fi) {
	printf("-----------------------------------------------------\n");
    fsstruct *file = (fsstruct *) fi->fh;
    printf("reading fi %s\n", file->name );
    printf("release: (path=%s)\n", path);
	return 0;
}

int lfs_make_file( const char *path, mode_t mode, dev_t wtf ) {
    printf("-----------------------------------------------------\n");
    printf("mknod: (path=%s)\n", path);
    printf( "mode_t is %u\ndev_t is %ld\n", mode, wtf);
    
    if ( !S_ISREG(mode) ) return -EACCES;

    fsstruct *folder = locate_parent_folder( path, root );

    char **dirs = str_split( '/', path );
    int n = str_count( '/', path );
    char *filename = dirs[n-1];

    fsstruct *new_file = fs_new_file( filename );
    if ( new_file == NULL ) { str_delete_split( dirs, n ); return -ENOMEM; }
    fs_add_fsstruct_to_folder( folder, new_file );
    
    time( &new_file->last_access_time );
    time( &new_file->last_modification_time );

    str_delete_split( dirs, n );    
    
    return 0;
}

int lfs_make_dir( const char *path, mode_t mode ) {
    printf("-----------------------------------------------------\n");
    printf("mkdir: (path=%s)\n", path);
    printf( "mode_t is %u\n", mode ); // Wtf is this.

    fsstruct *parent_folder = locate_parent_folder( path, root );
    if ( parent_folder == NULL ) return -ENOENT;

    char **dirs = str_split( '/', path );
    int n = str_count( '/', path );
    char *filename = dirs[n-1];

    fsstruct *new_folder = fs_new_folder( filename );
    if ( new_folder == NULL ) { str_delete_split( dirs, n ); return -ENOMEM; }
    fs_add_fsstruct_to_folder( parent_folder, new_folder );

    time( &new_folder->last_access_time );
    time( &new_folder->last_modification_time );

    str_delete_split( dirs, n );

    return 0;
}

int lfs_utimens( const char *path, struct utimbuf *times ) {
    printf("-----------------------------------------------------\n");
    printf("utime: (path=%s)\n", path);

    fsstruct *node = locate_folder_file( path, root );
    if ( node == NULL ) return -1;

    node->last_access_time = times->actime;
    node->last_modification_time = times->modtime;

    return 0;
}

int lfs_remove_file( const char *path ) {
    printf("-----------------------------------------------------\n");
    printf("unlink: (path=%s)\n", path);

    fsstruct *folder = locate_parent_folder( path, root );
    if ( folder == NULL ) return -ENOENT;
    fsstruct *file = locate_folder_file( path, root );
    if ( file == NULL ) return -ENOENT;

    printf( "Found parent folder: %s\n", folder->name );
    printf( "Found file         : %s\n", file->name );

    // unlink folder -> file
    for ( unsigned long i = 0; i < folder->max_entries; ++i ) {
        if ( folder->subdirs[i] != NULL ) {
            if ( folder->subdirs[i]->id == file->id ) {
                folder->subdirs[i] = NULL;
                folder->entries--;
                break;
            }
        }
    }

    // deallocate file
    fs_rm_file( file );    

    return 0;
}

int lfs_remove_folder( const char *path ) {
    printf("-----------------------------------------------------\n");
    printf("rmdir: (path=%s)\n", path);

    fsstruct *parent_folder = locate_parent_folder( path, root );
    if ( parent_folder == NULL ) return -ENOENT;
    fsstruct *folder = locate_folder_file( path, root );
    if ( folder == NULL ) return -ENOENT;

    if ( folder->entries != 0 ) return -1; // man 2 rmdir

    // unlink parent_folder -> folder
    for ( unsigned long i = 0; i < parent_folder->max_entries; ++i ) {
        if ( parent_folder->subdirs[i] != NULL ) {
            if ( parent_folder->subdirs[i]->id == folder->id ) {
                parent_folder->subdirs[i] = NULL;
                parent_folder->entries--;
                break;
            }
        }
    }

    fs_rm_folder( folder );
    return 0;
}

int lfs_truncate( const char* path, off_t size ) {
    printf("-----------------------------------------------------\n");
    printf("truncate: (path=%s)\n", path);
    printf( "size is %ld\n", size );
    // if ( size == 0 ) return -1;
    
    fsstruct *file = locate_folder_file( path, root );
    if ( file == NULL ) return -ENOENT;

    if ( file->type != FILE_T ) return -ENOENT;

    if ( file->data == NULL ) {
        printf( "Data field was not initialized. Allocating memory!\n");
        file->data = malloc( sizeof( char ) * size );
        if ( file->data == NULL ) return -ENOMEM;
        printf( "Allocated memory for the file %s\n", file->name );
    } else {
        printf( "The data field was already allocated.\n");
        char *tmp = realloc( file->data, size );
        if ( tmp == NULL && size != 0 ) return -ENOMEM;
        file->data = tmp;
        printf( "Truncated the data field.\n" );
    }

    if ( file->size > size )
        file->size = size;
    file->max_size = size;

    printf( "New file_size: %ld\nNew max_size: %ld\n", file->size, file->max_size );

    return 0;
}

int lfs_rename( const char* from, const char* to ) {
    printf("-----------------------------------------------------\n");
    printf("rename:\n\t(from=%s)\n\t(to  =%s)\n", from, to );

    fsstruct *from_folder = locate_parent_folder( from, root );
    fsstruct *to_folder = locate_parent_folder( to, root );
    fsstruct *node_to_move = locate_folder_file( from, root );

    if ( from_folder  == NULL ) return -ENOENT;
    if ( to_folder    == NULL ) return -ENOENT;
    if ( node_to_move == NULL ) return -ENOENT;

    printf( "from_folder: %s\n", from_folder->name );
    printf( "to_folder: %s\n", to_folder->name );
    printf( "node_to_move: %s\n", node_to_move->name );

    fsstruct *replace_node = locate_folder_file( to, root );
    if ( replace_node != NULL ) { // A node already exists at the target. Decide.
        printf( "Found %s at target\n", replace_node->name );
        if ( node_to_move->type != replace_node->type ) return -1;

        // We need to remove it
        if ( replace_node->type == FOLDER_T ) {
            // lfs_remove_folder( to );
            return -1;
        } else if ( replace_node->type == FILE_T ) {
            lfs_remove_file( to );
        } else {
            return -1; // Maybe something else.
        }
    }

    // Now we need to move the node to the destination.
    // unlink from current parent.
    for ( unsigned long i = 0; i < from_folder->max_entries; ++i ) {
        if ( from_folder->subdirs[i] != NULL ) {
            if ( from_folder->subdirs[i]->id == node_to_move->id ) {
                from_folder->subdirs[i] = NULL;
                from_folder->entries--;
                break;
            }
        }
    }

    // create new link at dest_parent
    fs_add_fsstruct_to_folder( to_folder, node_to_move );    

    // Set the new name.

    char *filename = malloc( sizeof( char ) * (strlen( to ) + 1) );
    strcpy( filename, to );
    filename = strtok( filename, "/" );
    char *old;
    while ( filename != NULL ) {
        old = filename;
        
        filename = strtok( NULL, "/" );
        printf( "filename %s\n", old );
    }

    node_to_move->name_size = strlen( old ) + 1;
    free( node_to_move->name );
    node_to_move->name = calloc( node_to_move->name_size, sizeof( char ) );
    if ( node_to_move->name == NULL ) {
        printf( "Lmao.... You are fucked haha...\nThe renaming didn't work. And it is too much work to fix this :)\n" );
        return -ENOMEM;
    }
    strcpy( node_to_move->name, old );
    
    free( filename );

    return 0;
}

/*  THIS IS A LINK. :0 ALMOST
int lfs_rename( const char* from, const char* to ) {
    printf("-----------------------------------------------------\n");
    printf("rename:\n\t(from=%s)\n\t(to  =%s)\n", from, to );

    fsstruct *from_folder = locate_parent_folder( from, root );
    fsstruct *to_folder = locate_parent_folder( to, root );
    fsstruct *node_to_move = locate_folder_file( from, root );

    if ( from_folder  == NULL ) return -ENOENT;
    if ( to_folder    == NULL ) return -ENOENT;
    if ( node_to_move == NULL ) return -ENOENT;

    printf( "from_folder: %s\n", from_folder->name );
    printf( "to_folder: %s\n", to_folder->name );
    printf( "node_to_move: %s\n", node_to_move->name );

    fsstruct *replace_node = locate_folder_file( to, root );
    if ( replace_node != NULL ) { // A node already exists at the target. Decide.
        printf( "Found %s at target\n", replace_node->name );
        if ( node_to_move->type != replace_node->type ) return -1;

        // We need to remove it
        if ( replace_node->type == FOLDER_T ) {
            // lfs_remove_folder( to );
        } else if ( replace_node->type == FILE_T ) {
            // lfs_remove_file( to );
        } else {
            return -1; // Maybe something else.
        }
    }

    // Now we need to move the node to the destination.

    // unlink from current parent.
    for ( int i = 0; i < from_folder->entries; ++i ) {
        if ( from_folder->subdirs[i] != NULL ) {
            if ( from_folder->subdirs[i]->id == node_to_move->id ) {
                // from_folder->subdirs[i] = NULL;
                // from_folder->entries--;
                break;
            }
        }
    }

    // create new link at dest_parent
    fs_add_fsstruct_to_folder( to_folder, node_to_move );

    return 0;
}

*/

int main( int argc, char *argv[] ) {
    // FILE *fd;
    // root = fs_new_folder( "" );
    // fsstruct *hello = fs_new_file( "hello.txt" );
    // hello->data = malloc( sizeof( char ) * 13);
    // memcpy(hello->data, "Hej med dig\n", 12);
    // hello->size = 13;
    // fsstruct *home = fs_new_folder( "Homeee" );
    // fsstruct *desktop = fs_new_folder( "Desktop" );
    // fs_add_fsstruct_to_folder( home, hello );
    // fs_add_fsstruct_to_folder( root, home );
    // fs_add_fsstruct_to_folder( root, desktop );

    // fd = fopen( "disk.img", "wb" );
    // if ( fd == NULL ) printf("file null???\n");
    // printf("Opened file\n");
    // printf("Wrote %ld bytes\n", disk_write_folder( fd, root ) );
    
    
    // fclose(fd);

    root = disk_read( "disk.img" );
    if ( root == NULL ) {
        printf( "WOPS\n");
        return -1;
    }

    // printf( "%ld\n", root->entries );
    // root->entries = 0;
    // printf( "%ld\n", root->entries );
	fuse_main( argc, argv, &lfs_oper );
    
    disk_write( "disk.img", root );

    fsdealloc( root );

    return 0;
}