#include <errno.h>
#include <fuse.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "easteregg.h"
#include "chess/moves.h"
#include "chess/chess.h"
#include "chess/datastructures.h"
#include "utils.h"
#include "fsstructure.h"
#include "lfs.h"

void create_move_folder( Move *move, void *parent_f ) {
    fsstruct *parent = (fsstruct *)parent_f;

    // Create the folder
    char *filename = malloc( sizeof( char ) * 5 );
    if ( filename == NULL ) return;

    filename[0] = get_col( move->from );
    filename[1] = '1' + get_row( move->from );
    filename[2] = get_col( move->to );
    filename[3] = '1' + get_row( move->to );
    filename[4] = '\0';
    // printf( "Constructed filename %s\n", filename );
    fsstruct *move_folder = fs_new_folder( filename );
    if ( move_folder == NULL ) return;

    fs_add_fsstruct_to_folder( parent, move_folder );
    free( filename );

}


int easter_readdir( const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, fsstruct *root ) {
	(void) offset;
	(void) fi;
    printf("-----------------------------------------------------\n");
	printf("readdir-EasterEdition: (path=%s)\n", path);
    
    fsstruct *folder = locate_folder_file( path, root );
    if ( folder == NULL ) return -ENOENT;


    // Check if we already haven't made touched this file.
    if ( folder->entries == 0 ) {
        // remove the /chess/ from path
        char *move_sequence = malloc( sizeof( char ) * (1 + strlen( path ) - 7) );
        if ( move_sequence == NULL ) return -ENOMEM;

        strcpy( move_sequence, path + 7 );
        chess_board *cb = chess_new_board_from_path( move_sequence);

        // Make the board file
        fsstruct *board_file = fs_new_file( "board" );
        if ( board_file == NULL ) { free( move_sequence ); free( cb->board ); free( cb ); return -ENOMEM; }

        fs_add_fsstruct_to_folder( folder, board_file );

        char *ascii = chess_to_ascii( cb );
        // Write the ascii code...
        
        board_file->size = strlen( ascii );
        board_file->data = calloc( board_file->size, sizeof( char ) );
        if ( board_file->data == NULL ) {  free( move_sequence ); free( cb->board ); free( cb ); free( ascii ); return -ENOMEM; }
        strcpy( board_file->data, ascii );
        
        // Create all the moves dirs.
        List *moves = get_all_moves( cb );

        list_foreach_with_data1( moves, &create_move_folder, folder );


        list_delete( moves, &move_delete );
        free( ascii );
        free( move_sequence );
        free( cb->board );
        free( cb );
    }
    

    filler( buf, ".", NULL, 0 );
	filler( buf, "..", NULL, 0 );
   
    for ( unsigned long i = 0; i < folder->max_entries; ++i ) {
        
        if ( folder->subdirs[i] != NULL ) {
            filler( buf, folder->subdirs[i]->name, NULL, 0 );
        }
    }

	return 0;
}
