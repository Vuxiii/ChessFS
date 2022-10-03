#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "chess.h"
#include "moves.h"
#include "datastructures.h"

chess_board *chess_new_board() {
    chess_board *cb = malloc( CBSIZE );
    if ( cb == NULL ) return NULL;

    cb->board = calloc( sizeof( chess_piece ), 64 );
    if ( cb->board == NULL ) { free( cb ); return NULL; }
    cb->is_white = 1;

    // Setup black side
    // printf( "%d\n", get_index( 'A', 8 ));
    cb->board[ get_index( 'A', 7 ) ] = create_piece( BLACK, ROOK );
    cb->board[ get_index( 'H', 7 ) ] = create_piece( BLACK, ROOK );
    cb->board[ get_index( 'B', 7 ) ] = create_piece( BLACK, KNIGHT );
    cb->board[ get_index( 'G', 7 ) ] = create_piece( BLACK, KNIGHT );
    cb->board[ get_index( 'C', 7 ) ] = create_piece( BLACK, BISHOP );
    cb->board[ get_index( 'F', 7 ) ] = create_piece( BLACK, BISHOP );
    cb->board[ get_index( 'D', 7 ) ] = create_piece( BLACK, QUEEN );
    cb->board[ get_index( 'E', 7 ) ] = create_piece( BLACK, KING );
    cb->board[ get_index( 'A', 6 ) ] = create_piece( BLACK, PAWN );
    cb->board[ get_index( 'B', 6 ) ] = create_piece( BLACK, PAWN );
    cb->board[ get_index( 'C', 6 ) ] = create_piece( BLACK, PAWN );
    cb->board[ get_index( 'D', 6 ) ] = create_piece( BLACK, PAWN );
    cb->board[ get_index( 'E', 6 ) ] = create_piece( BLACK, PAWN );
    cb->board[ get_index( 'F', 6 ) ] = create_piece( BLACK, PAWN );
    cb->board[ get_index( 'G', 6 ) ] = create_piece( BLACK, PAWN );
    cb->board[ get_index( 'H', 6 ) ] = create_piece( BLACK, PAWN );

    // Setup White side
    cb->board[ get_index( 'A', 0 ) ] = create_piece( WHITE, ROOK );
    cb->board[ get_index( 'H', 0 ) ] = create_piece( WHITE, ROOK );
    cb->board[ get_index( 'B', 0 ) ] = create_piece( WHITE, KNIGHT );
    cb->board[ get_index( 'G', 0 ) ] = create_piece( WHITE, KNIGHT );
    cb->board[ get_index( 'C', 0 ) ] = create_piece( WHITE, BISHOP );
    cb->board[ get_index( 'F', 0 ) ] = create_piece( WHITE, BISHOP );
    cb->board[ get_index( 'D', 0 ) ] = create_piece( WHITE, QUEEN );
    cb->board[ get_index( 'E', 0 ) ] = create_piece( WHITE, KING );
    cb->board[ get_index( 'A', 1 ) ] = create_piece( WHITE, PAWN );
    cb->board[ get_index( 'B', 1 ) ] = create_piece( WHITE, PAWN );
    cb->board[ get_index( 'C', 1 ) ] = create_piece( WHITE, PAWN );
    cb->board[ get_index( 'D', 1 ) ] = create_piece( WHITE, PAWN );
    cb->board[ get_index( 'E', 1 ) ] = create_piece( WHITE, PAWN );
    cb->board[ get_index( 'F', 1 ) ] = create_piece( WHITE, PAWN );
    cb->board[ get_index( 'G', 1 ) ] = create_piece( WHITE, PAWN );
    cb->board[ get_index( 'H', 1 ) ] = create_piece( WHITE, PAWN );

    return cb;
}

chess_board *chess_new_board_from_path( char *path ) {
    chess_board *cb = chess_new_board();
    if ( cb == NULL ) return NULL;


    char *move = strtok( path, "/" );

    while( move != NULL ) {
        printf( "Move->[%c%c]->[%c%c]\n", move[0], move[1], move[2], move[3] );
        int from_row = move[1] - 49;
        int to_row = move[3] - 49;
        int from_index = get_index( move[0], from_row );
        int to_index = get_index( move[2], to_row );
        Move *actual_move = move_new( from_index, to_index, get_piece( cb, from_index ) );
        printf( "from: %d\n", from_index );
        printf( "from_row: %d\n", from_row );
        printf( "from_col: %c\n", move[0] );
        printf( "to: %d\n", to_index );
        printf( "to_row: %d\n", to_row );
        printf( "to_col: %c\n", move[2] );
        make_move( cb, actual_move );
        
        free( actual_move );
        move = strtok( NULL, "/" );
    }
    
    return cb;
}

chess_piece create_piece( int color, int piece ) {
    if ( color == WHITE )
        piece = piece | COLOR;
    return piece;
}

chess_piece get_piece( chess_board *cb, int index ) {
    return cb->board[index];
}

int check_set( chess_piece piece, unsigned int bit ) {
    if ( bit == 0 )
        return piece & 1;
    else 
        return piece & (1 << bit);
}

int get_type( chess_piece piece ) {
    // printf( "Piece: %d\n", piece );
    // piece = piece & (255 << 3);
    // printf( "Piece: %d\n", piece );
    // printf( "%d", piece );
    if ( check_set( piece, 0 ) && !check_set( piece, 1 ) && !check_set( piece, 2 ) )
        return PAWN;
    else if ( !check_set( piece, 0 ) && check_set( piece, 1 ) && !check_set( piece, 2 ) )
        return ROOK;
    else if ( check_set( piece, 0 ) && check_set( piece, 1 ) && !check_set( piece, 2 ) )
        return BISHOP;
    else if ( !check_set( piece, 0 ) && !check_set( piece, 1 ) && check_set( piece, 2 ) )
        return KNIGHT;
    else if ( check_set( piece, 0 ) && !check_set( piece, 1 ) && check_set( piece, 2 ) )
        return QUEEN;
    else
        return KING;
}

int is_white( chess_piece piece ) {
    return (piece & COLOR) >> 3 == 1;
}

int get_index( char col, int row ) {
    // printf( "index: %d\n", ((col - 'H') + row * 8) - 1);
    return ((col - 'A') + row * 8);
}

int get_row( int index ) {
    return index / 8;
}

char get_col( int index ) {
    return 'A' + (index % 8);
}

void print_piece( chess_piece piece ) {
    switch (get_type( piece ))
    {
    case PAWN:
        printf( "Pawn " );
        break;
    case ROOK:
        printf( "Rook " );
        break;
    case BISHOP:
        printf( "Bishop " );
        break;
    case KNIGHT:
        printf( "Knight " );
        break;
    case QUEEN:
        printf( "Queen " );
        break;
    case KING:
        printf( "King " );
        break;
    default:
        break;
    }
    if ( is_white( piece ) )
        printf( "White\n" );
    else
        printf( "Black\n" );
}

List *collect_all_pieces( chess_board *cb ) {
    List *pieces = NULL;

    for ( int i = 0; i < 64; ++i ) {
        if ( cb->board[i] != 0 ) {
            list_add( &pieces, create_piece_entry( cb->board[i], i ) );
        }
    }
    return pieces;
}

piece_info *create_piece_entry( chess_piece piece, int index ) {
    piece_info *pi = malloc( sizeof( piece_info ) );
    if ( pi == NULL ) return NULL;
    pi->loc = index;
    pi->piece = piece;
    return pi;
}

void delete_piece_entry( piece_info *pi ) {
    free( pi );
}

void make_move( chess_board *cb, Move *move ) {
    chess_piece piece = cb->board[ move->from ];
    cb->board[ move->from ] = 0;
    cb->board[ move->to ] = piece;
    cb->is_white = (cb->is_white + 1) % 2;
}

/*
  |A |B |C |D |E |F |G |H |
--+--+--+--+--+--+--+--+--+--
 8|  |  |  |  |  |  |  |  |8
--+--+--+--+--+--+--+--+--+--
 7|  |  |  |  |  |  |  |  |7
--+--+--+--+--+--+--+--+--+--
 6|  |  |  |  |  |  |  |  |6
--+--+--+--+--+--+--+--+--+--
 5|  |  |  |  |  |  |  |  |5
--+--+--+--+--+--+--+--+--+--
 4|  |  |  |  |  |  |  |  |4
--+--+--+--+--+--+--+--+--+--
 3|  |  |  |  |  |  |  |  |3
--+--+--+--+--+--+--+--+--+--
 2|  |  |  |  |  |  |  |  |2
--+--+--+--+--+--+--+--+--+--
 1|  |  |  |  |  |  |  |  |1
--+--+--+--+--+--+--+--+--+--
  |A |B |C |D |E |F |G |H |
*/
 


char *chess_to_ascii( chess_board *cb ) {
    char *out = calloc( sizeof( char ), 550 );
    if ( out == NULL ) return NULL;
    strcat( out, "  |H |G |F |E |D |C |B |A |" );

    int i = 63;
    for ( int _ = 0; _ < 18; _++ ) {
        if ( _ % 2 == 0 ) {
            strcat( out, "\n--+--+--+--+--+--+--+--+--+--\n " );
            int row = get_row( i ) + 1;
            char c = '0' + row;
            size_t len = strlen( out );
            out[ len ] = c;
            out[ len + 1 ] = '|';
            
        } else {
            for ( int __ = 0; __ < 8; __++ ) {
                if ( i < 0 ) break;
                if ( cb->board[i] == 0 ) {
                    strcat( out, "  |" );
                } else {
                    chess_piece p = get_piece( cb, i );
                    char *out2;
                    if ( is_white( p ) ) {
                        if ( get_type( p ) == PAWN )
                            out2 = "wP|";
                        else if ( get_type( p ) == ROOK )
                            out2 = "wR|";
                        else if ( get_type( p ) == KING )
                            out2 = "wK|";
                        else if ( get_type( p ) == QUEEN )
                            out2 = "wQ|";
                        else if ( get_type( p ) == KNIGHT )
                            out2 = "wN|";
                        else 
                            out2 = "wB|";
                    } else {
                        if ( get_type( p ) == PAWN )
                            out2 = "bP|";
                        else if ( get_type( p ) == ROOK )
                            out2 = "bR|";
                        else if ( get_type( p ) == KING )
                            out2 = "bK|";
                        else if ( get_type( p ) == QUEEN )
                            out2 = "bQ|";
                        else if ( get_type( p ) == KNIGHT )
                            out2 = "bN|";
                        else 
                            out2 = "bB|";
                    }
                    strcat( out, out2 );
                }
                i--;
            }
        }
    }
    out[522] = '\0';
    return out;
}

// int main( void ) {
//     setlocale(LC_ALL,"");
//     // chess_board *cb = chess_new_board();
//     // List *l = get_all_moves( cb );

//     // list_foreach( l, &print_move );

//     // wchar_t *t = chess_to_ascii( cb );

//     // printf( "%ls\n", t );

//     // free( t );

//     // list_delete( l, &move_delete );
//     // free( cb->board );
//     // free( cb );

//     char *path = malloc( sizeof( char ) * 15 );
//     strcpy( path, "B1A3/E2E3/B7B6" ); //
//     chess_board *cb = chess_new_board_from_path( path );
//     // chess_board *cb = chess_new_board();

//     wchar_t *t = chess_to_ascii( cb );

//     printf( "%ls\n", t );

//     free( t );

//     free( cb->board );
//     free( cb );
//     free( path );
//     return 0;

// }