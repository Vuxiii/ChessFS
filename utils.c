#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "fsstructure.h"

char **str_split( const char c, const char *str ) {
    int n = str_count(c, str);
    
    char **out = calloc( n, sizeof( char *) );
    if ( out == NULL ) return NULL;
    printf("Splitted %s into %d entries\n", str, n);
    size_t left = 1;
    size_t right = 0;
    for ( int current = 0; current != n; ++current ) {
        // printf("Loop 18\n");
        // Find the right wall
        for ( right = left; str[right] != c && right < strlen( str ); ++right ) { /*printf("%c was not %c at index %d\n", str[right], c, right );*/ } // 
        int size = right - left;
        
        out[current] = calloc(size, sizeof( char ) );
        // Free all the allocated memory.
        if ( out[current] == NULL ) { 
            for ( int i = current-1; i >= 0; --i ) {
                free( out[i] );
            }
            free( out ); 
            return NULL;
        }
        printf("current is %d\n", current );
        memcpy( out[current], str + left, size );
        out[current][size] = '\0';
        printf( "Size for %s is %d\n", out[current], size );
        // printf("%s\n", out[current]);
        left = right+1;
    }
    return out;
}

int str_count( const char c, const char *str ) {
    int n = 0;
    for ( size_t i = 0; i != strlen( str ); ++i ) {
        n = str[i] == c ? n+1 : n;
    }
    return n;
}

void str_delete_split( char **s, int size ) {   
    for ( int i = 0; i < size; ++i ) {
        free(s[i]);
    }
    free(s);
}

fsstruct *locate_parent_folder( const char *path, fsstruct *root ) {
    fsstruct *node = root;
    printf( "Locate_parent_folder path: %s\n", path );
    char **ent = str_split( '/', path );
    int n = str_count( '/', path );
    printf("Node has %ld entries, n is %d\n", node->entries, n );
    printf( "Traversing the path %s\n", path );
    if ( node->entries == 0 ) { str_delete_split( ent, n ); return node; }
    for ( int i = 0; i < n-1; ++i ) {
        // Pick child
        printf("Pick Child %d\n", i);
        
        for ( unsigned long id = 0; id < node->max_entries; ++id ) {
            if ( node->subdirs[id] != NULL ) {
                printf( "Checking %s == %s\n", node->subdirs[id]->name, ent[i] );
                if ( strcmp( node->subdirs[id]->name, ent[i] ) == 0 ) {
                    node = node->subdirs[id];
                    break;
                }
            }
        }
    }

    str_delete_split( ent, n );
    printf( "Picked folder %s\n", node->name );
    return node;
}


fsstruct *locate_folder_file( const char *path, fsstruct *root ) {
    fsstruct *parent = locate_parent_folder( path, root );
    if ( parent == NULL ) return NULL;

    char **folders = str_split( '/', path );
    if ( folders == NULL ) return NULL;
    char *filename = folders[ str_count( '/', path ) -1 ];
    
    fsstruct *folder_file = NULL;
    for ( unsigned long i = 0; i < parent->max_entries; i++ ) {
        if ( parent->subdirs[i] != NULL ) {
            if ( strcmp( parent->subdirs[i]->name, filename ) == 0 ) {
                folder_file = parent->subdirs[i];
                break;
            }
        }
    }

    str_delete_split( folders, str_count( '/', path ) );
    return folder_file;
}


fsstruct *locate_file( const char *filename, fsstruct *folder ) { // NOT USED
    fsstruct *file = NULL;
    printf("Trying to locate file %s in folder: %s\n", filename, folder->name );
    for ( unsigned long i = 0; i < folder->max_entries; ++i ) {
        // printf( "Checking index %d\n", i );
        if ( folder->subdirs[i] != NULL ) {
            printf("Checking %s == %s\n", folder->subdirs[i]->name, filename );
            if ( strcmp( folder->subdirs[i]->name, filename ) == 0 ) {
                printf( "Found file %s\n", filename );
                file = folder->subdirs[i];
                break;
            }
        }
    }

    return file;
}


int file_read( fsstruct *file, char *buffer, size_t size, off_t offset ) {
    // Check if data is not null
    if ( file->data == NULL ) return -ENODATA;
    
    
    // Limit size to the length of the buffer of file data.
    if ( size > file->size ) size = file->size;

    if ( offset + size > file->size ) size = file->size - (offset + size);

    // Copy into the buffer

    memcpy( buffer, file->data + offset, size );
    
    // return amount read.
    return size;
}

int file_write( fsstruct *file, const char *buffer, size_t size, off_t offset ) {
    if ( file->data == NULL ) {
        printf( "Allocating memory for file %s\n", file->name );
        file->data = malloc( sizeof( char ) * size );
        if ( file->data == NULL ) return -ENOMEM;

        printf( "Memory allocated for file %s\n", file->name );
    }
    if ( size + offset > file->max_size ) {
        size_t new_size = file->max_size;
        while (new_size < size + offset ) new_size += FILE_DEFAULT_SIZE;
        char *tmp = realloc( file->data, new_size );
        if ( tmp == NULL ) return -ENOMEM;
        file->data = tmp;
        file->max_size = new_size;
    }
    printf("Trying to write at offset %ld, %ld amount of bytes\n", offset, size );
    memcpy( file->data + offset, buffer, size );
    printf( "Wrote to file %s\n", file->name );
    if ( offset + size > file->size )
        file->size = offset + size;
    return size;
}

fsstruct *disk_read( char *file ) {
    fsstruct *root;
    if ( access( file, F_OK ) != 0 ) {
        // No filesystem stored.
        root = fs_new_folder( "" );
        if ( root == NULL ) return NULL;
        disk_write( file, root );
    } else {
        // Read already existing filesystem.
        FILE *fd = fopen( file, "rb" );

        if ( fd == NULL ) return NULL;
        root = disk_read_folder( fd );
        fclose( fd );
        if ( root == NULL ) {
            printf( "For some reason the disk_read did not succeed\n" );
            return NULL;
        }
    }
     
    return root;
}

int disk_write( char *file, fsstruct *root ) {
    FILE *fd = fopen( file, "wb" );
    if ( fd == NULL ) return -1;
    printf("Opened file\n");
    printf("Wrote %ld bytes\n", disk_write_folder( fd, root ) );
    fclose(fd);
    return 0;
}

void fsdealloc( fsstruct *root ) {
    for ( long unsigned i = 0; i < root->max_entries; ++i ) {
        if ( root->subdirs[i] != NULL ) {
            if ( root->subdirs[i]->type == FOLDER_T )
                fsdealloc( root->subdirs[i] ); 
            else 
                fs_rm_file( root->subdirs[i] );            
        }
    }
    fs_rm_folder( root );
} 

// int main( void ) {
//     char **str = str_split( '/', "Hej/Med/Dig" );
//     // printf("%d\n", sizeof(*str)/sizeof(char*));
//     for ( int i = 0; i < str_count('/', "Hej/Med/Dig")+1; i++ ) {
//         printf("%s\n", str[i] );
//     }
//     return 0;
// }