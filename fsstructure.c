#include "fsstructure.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdatomic.h>
#include <stdio.h>
#include <unistd.h>

_Atomic unsigned long _ids = 0; 

fsstruct *fs_new_file( char *f_name ) {
    fsstruct *file = malloc( FSSTSIZE );
    if ( file == NULL ) return NULL;
    

    file->name_size = strlen( f_name )+1;
    file->name = malloc( sizeof( char ) * file->name_size );
    if ( file->name == NULL ) { free(file); return NULL; }
    
    strcpy( file->name, f_name );
    
    file->type = FILE_T;
    file->id = get_id();
    
    file->last_access_time = 0;
    file->last_modification_time = 0;

    file->data = NULL;
    file->subdirs = NULL;
    file->size = 0;
    file->max_size = FILE_DEFAULT_SIZE;
    
    file->entries = 0;
    file->max_entries = 0;

    return file;
}

void fs_rm_file( fsstruct *file ) {
    if ( file->data != NULL ) free( file->data );
    free( file->name );
    free( file );
}

fsstruct *fs_new_folder( char *f_name ) {
    fsstruct *folder = malloc( FSSTSIZE );
    if ( folder == NULL ) return NULL;

    folder->name_size = strlen( f_name )+1;
    folder->name = malloc( sizeof(char) * folder->name_size );
    if ( folder->name == NULL ) { free(folder); return NULL; }
    strcpy( folder->name, f_name );
    
    folder->type = FOLDER_T;
    folder->id = get_id();

    folder->last_access_time = 0;
    folder->last_modification_time = 0;

    folder->subdirs = calloc( sizeof(fsstruct *), MAX_ENTRIES );
    if ( folder->subdirs == NULL ) { free(folder->name); free(folder); return NULL; }
    
    folder->data = NULL;
    folder->size = 0;
    folder->max_size = 0;

    folder->entries = 0;
    folder->max_entries = MAX_ENTRIES;

    // for ( int i = 0; i < folder->max_entries; ++i ) folder->subdirs[i] = NULL;

    return folder;
}

void fs_rm_folder( fsstruct *folder ) {
    free( folder->name );
    free( folder->subdirs );
    free( folder );
}

int fs_add_fsstruct_to_folder( fsstruct *folder, fsstruct *node ) {
    if ( folder->entries == folder->max_entries ) {
        fsstruct **new_loc = realloc( folder->subdirs, sizeof(fsstruct *) * folder->max_entries*2 );
        if ( new_loc == NULL ) return -ENOMEM;
        folder->subdirs = new_loc;
        folder->max_entries *= 2;
        for ( unsigned long i = folder->entries; i < folder->max_entries; i++ ) 
            folder->subdirs[i] = NULL;
    }
    // Find empty spot.
    for ( unsigned long i = 0; i < folder->max_entries; ++i ) {
        if ( folder->subdirs[i] == NULL ) {
            folder->subdirs[i] = node;
            folder->entries += 1;
            break;
        }
    }
    
    printf("%s: folder->entries = %ld\n",folder->name, folder->entries );
    printf("%s: folder->max_ent = %ld\n",folder->name, folder->max_entries );
    printf( "The added folders name is %s\n", node->name );
    // printf("folder->entries = %ld\n", folder->entries );
    return 0;
}

size_t disk_write_file( FILE *fd, fsstruct *file ) {
    printf("--------------------------------------------\n");
    printf( "Start of write-file: '%s'\n", file->name );
    printf( "Offset in file: %ld\n", ftell( fd ) );

    // type
    // name size
    // name
    // size
    // max_size
    // data
    // id
    // laccess
    // lmod
    size_t sum = 0;
    sum += fwrite( &file->type,                   sizeof( file->type ),             1, fd );
    printf("Wrote type %d\n", file->type);
    sum += fwrite( &file->name_size,              sizeof( file->name_size ),        1, fd );
    printf("Wrote name size %ld\n", file->name_size);
    sum += fwrite( file->name,                   sizeof( char ), file->name_size, fd );
    printf("Wrote name %s\n", file->name);
    sum += fwrite( &file->size,                   sizeof( file->size ),             1, fd );
    printf("Wrote size %ld\n", file->size);
    sum += fwrite( &file->max_size,               sizeof( file->max_size ),         1, fd );
    printf("Wrote max_size %ld\n", file->max_size);
    sum += fwrite( file->data,                   sizeof( char ),      file->size, fd );
    printf("Wrote data %s\n", file->data);
    sum += fwrite( &file->id,                     sizeof( file->id ),               1, fd );
    printf("Wrote id %ld\n", file->id);
    sum += fwrite( &file->last_access_time,       sizeof( file->last_access_time ), 1, fd );
    printf("Wrote access %ld\n", file->last_access_time);
    sum += fwrite( &file->last_modification_time, sizeof( file->last_modification_time ), 1, fd );
    printf("Wrote modification %ld\n", file->last_modification_time );

    printf( "Stop of write-file: '%s'\n", file->name );
    printf("--------------------------------------------\n");

    return sum;
}

fsstruct *disk_read_file( FILE *fd ) {
    printf("--------------------------------------------\n");
    printf( "Start of read-file\n" );
    printf( "Offset in file: %ld\n", ftell( fd ) );
    fsstruct *file = malloc( FSSTSIZE );
    if ( file == NULL ) return NULL;


    // read the fs type
    
    fread( &file->type, sizeof(file->type), 1, fd );
    printf( "Type:\t%d\n", file->type );
    
    // read the length of the name.

    
    fread( &file->name_size, sizeof(file->name_size), 1, fd );
    printf( "name_size:\t%ld\n", file->name_size );
    
    file->name = calloc( file->name_size, sizeof(char) );
    if ( file->name == NULL ) { free(file); return NULL; }

    fread( file->name, sizeof(char), file->name_size, fd );
    printf( "name:\t%s\n", file->name );

    fread( &file->size, sizeof(file->size), 1, fd );
    printf( "size:\t%ld\n", file->size );
    
    fread( &file->max_size, sizeof(file->max_size), 1, fd );

    printf( "max_size:\t%ld\n", file->max_size );

    // Read the data
    // file->data = malloc( sizeof( char ) * file->size );
    file->data = calloc( file->max_size, sizeof( char ) );
    if ( file->data == NULL ) { 
        printf( "Could not allocate for file->data\n");
        free( file->name ); free( file ); return NULL; }

    fread( file->data, sizeof(char), file->size, fd );

    printf( "data:\t%s\n", file->data );
    // Read id

    fread( &file->id, sizeof(file->id), 1, fd );
    printf( "id:\t%ld\n", file->id );
    
    fread( &file->last_access_time, sizeof(file->last_access_time), 1, fd );
    printf( "last_access_time:\t%ld\n", file->last_access_time );

    // Read last_modification_time

    fread( &file->last_modification_time, sizeof(file->last_modification_time), 1, fd );
    printf( "last_modification_time:\t%ld\n", file->last_modification_time );
    
    file->entries = 0;
    file->max_entries = 0;
    file->subdirs = NULL;

    
    printf( "Stop of read-file\n" );
    printf("--------------------------------------------\n");

    return file;
}

size_t disk_write_folder( FILE *fd, fsstruct *folder ) {
    
    printf("--------------------------------------------\n");
    printf( "Start of write-folder: '%s'\n", folder->name );
    printf( "Offset in file: %ld\n", ftell( fd ) );
    // type
    // name size
    // name
    
    // id
    // laccess
    // lmod

    // entries
    // max_entries
    // subdirs
    size_t sum = 0;
    sum += fwrite( &folder->type,                   sizeof( folder->type ),             1, fd );
    printf("Wrote type %d\n", folder->type);
    
    sum += fwrite( &folder->name_size,              sizeof( folder->name_size ),        1, fd );
    printf("Wrote name size %ld\n", folder->name_size);
    
    // folder->name_size = 100;

    sum += fwrite( folder->name,                   sizeof( char ), folder->name_size, fd );
    printf("Wrote name %s\n", folder->name);
    
    sum += fwrite( &folder->id,                     sizeof( folder->id ),               1, fd );
    printf("Wrote id %ld\n", folder->id);
    
    sum += fwrite( &folder->last_access_time,       sizeof( folder->last_access_time ), 1, fd );
    printf("Wrote access\n");
    
    sum += fwrite( &folder->last_modification_time, sizeof( folder->last_modification_time ), 1, fd );
    printf("Wrote modification\n");
    
    sum += fwrite( &folder->entries,                   sizeof( folder->entries ),             1, fd );
    printf("Wrote entries %ld\n", folder->entries);
    sum += fwrite( &folder->max_entries,               sizeof( folder->max_entries ),         1, fd );
    printf("Wrote max_entries %ld\n", folder->max_entries);
    
    // sum += fwrite( &folder->data,                   sizeof( char ) * folder->size,      1, fd );
    // printf("Wrote data %s\n", folder->data);
    
    long entries_pos = ftell( fd );
    long offsets[folder->entries];
    int types[folder->entries];

    fseek( fd, sizeof( offsets ) + sizeof( types ), SEEK_CUR );
    
    printf( "Starting to write '%s's files and folders\n", folder->name );

    int current_offset = 0; 
    for ( unsigned long i = 0; i < folder->max_entries; ++i ) {
        
        if ( folder->subdirs[i] != NULL ) {
            fsstruct *node_to_write = folder->subdirs[i];
            printf( "Found '%s/%s', starting to write\n", folder->name, node_to_write->name );
            // Store the position to where the file / folder will be written.
            offsets[current_offset] = ftell( fd );
            if ( node_to_write->type == FOLDER_T ) {
                sum += disk_write_folder( fd, node_to_write );
                types[current_offset] = FOLDER_T;
            } else {
                sum += disk_write_file( fd, node_to_write );
                types[current_offset] = FILE_T;
            }
            current_offset++;
        }
    }

    long end_of_write_pos = ftell( fd );
    fseek( fd, entries_pos, SEEK_SET );

    // Write the offsets for each subdir in this folder.
    sum += fwrite( offsets, sizeof( offsets ), 1, fd );
    sum += fwrite( types, sizeof( types ), 1, fd );

    for ( unsigned long i = 0; i < folder->entries; ++i )
        printf( "offsets[%ld] = %ld\n", i, offsets[i] );
    for ( unsigned long i = 0; i < folder->entries; ++i )
        printf( "types[%ld] = %d\n", i, types[i] );

    fseek( fd, end_of_write_pos, SEEK_SET );

     
    printf( "Stop of write-folder: '%s'\n", folder->name );
    printf("--------------------------------------------\n");

    // free( offsets );
    // free( types );

    return sum;
}

fsstruct *disk_read_folder( FILE *fd ) {
    printf("--------------------------------------------\n");
    printf( "Start of read-folder\n" );
    printf( "Offset in file: %ld\n", ftell( fd ) );
    fsstruct *folder = malloc( FSSTSIZE );
    if ( folder == NULL ) {
        printf("Could not allocate folder\n");
        return NULL;
    }

    fread( &folder->type, sizeof( folder->type ), 1, fd );
    fread( &folder->name_size, sizeof( folder->name_size ), 1, fd );
    
    // folder->name_size = 10;
    
    folder->name = malloc( folder->name_size * sizeof( char ) );
    if ( folder->name == NULL ) { free( folder ); return NULL; }
    
    fread( folder->name, sizeof( char ), folder->name_size, fd );
    
    fread( &folder->id, sizeof( folder->id ), 1, fd );
    _ids = folder->id > _ids ? folder->id : _ids;
    fread( &folder->last_access_time, sizeof( folder->last_access_time ), 1, fd );
    fread( &folder->last_modification_time, sizeof( folder->last_modification_time ), 1, fd );
    
    unsigned long entries;
    
    fread( &entries, sizeof( folder->entries ), 1, fd );
    fread( &folder->max_entries, sizeof( folder->max_entries ), 1, fd );

    folder->subdirs = malloc( sizeof(fsstruct *) * folder->max_entries );
    if ( folder->subdirs == NULL ) { 
        printf( "Could not allocate subdirs\n");
        printf("max_entries %ld\n", folder->max_entries);
        free(folder->name); free(folder); return NULL; }

    printf( "Read type %d\n", folder->type );
    // printf( "Wtf %d\n", strlen(folder->name) );
    printf( "Read name size %ld\n", folder->name_size );


    printf( "Read name %s\n", folder->name );
    
    printf( "Read id %ld\n", folder->id );
    printf( "Read access %ld\n", folder->last_access_time );
    printf( "Read modification %ld\n", folder->last_modification_time );
    printf( "Read entries %ld\n", entries );
    printf( "Read max_entries %ld\n", folder->max_entries );

    long offsets[entries];
    int types[entries];

    fread( offsets, sizeof( offsets ), 1, fd );
    fread( types, sizeof( types ), 1, fd );
    

    for ( unsigned long i = 0; i < entries; ++i )
        printf( "offsets[%ld] = %ld\n", i, offsets[i] );
    for ( unsigned long i = 0; i < entries; ++i )
        printf( "types[%ld] = %d\n", i, types[i] );

    for ( int i = 0; i < entries; ++i ) {
        printf( "Types[%d] = %d\n", i, types[i] );
        fseek( fd, offsets[i], SEEK_SET );
        fsstruct *node;
        
        if ( types[i] == FOLDER_T ) {
            node = disk_read_folder( fd );
        } else {
            node = disk_read_file( fd );
        }
        if ( node == NULL ) { // FREE EVERYTHING
            for ( int j = i-1; j >= 0; --j ) {
                if ( types[i] == FOLDER_T )
                    fs_rm_folder( node );
                else
                    fs_rm_file( node );
            }
            printf( "Could not read node \n");
            return NULL;
        }
        _ids = node->id > _ids ? node->id : _ids;
        printf( "Read '%s/%s'\n", folder->name, node->name );
        fs_add_fsstruct_to_folder( folder, node );
        printf( "Added '%s' to parent folder '%s'\n", node->name, folder->name );
    }

    printf( "Stop of read-folder\n" );
    printf("--------------------------------------------\n");
    return folder;
}

unsigned long get_id() {
    return _ids++;
}