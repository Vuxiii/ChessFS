#include <stdio.h>

#include "moves.h"
#include "chess.h"


void printer( void *piece ) {
    chess_piece p = *(chess_piece *)piece;

    // printf( "Piece: %d\n", piece );
    print_piece( p );
}

void print_move( Move *move ) {
    printf( "[%c%d] -> [%c%d]\n", get_col( move->from ), get_row( move->from ), get_col( move->to ), get_row( move->to ) );
}

List *get_all_moves( chess_board *cb ) {
    List *pieces = collect_all_pieces( cb );
    List *moves = NULL;
    list_foreach_with_data2( pieces, &get_moves_for_index, cb, &moves );

    list_delete( pieces, &delete_piece_entry );

    return moves;
}

void get_moves_for_index( piece_info *pi, chess_board *cb, List **moves ) {
    if ( is_white( pi->piece ) != cb->is_white ) return;
    switch ( get_type( pi->piece ) )
    {
    case PAWN:
        get_pawn_moves( cb, pi->loc, moves );
        break;
    case ROOK:
        get_rook_moves( cb, pi->loc, moves );
        break;
    case BISHOP:
        get_bishop_moves( cb, pi->loc, moves );
        break;
    case KNIGHT:
        get_knight_moves( cb, pi->loc, moves );
        break;
    case QUEEN:
        get_queen_moves( cb, pi->loc, moves );
        break;
    case KING:
        get_king_moves( cb, pi->loc, moves );
        break;
    
    default:
        break;
    }
}

void get_pawn_moves( chess_board *cb, int index, List **moves ) {
    int piece_type = get_type( get_piece( cb, index ) );
    if ( is_white( cb->board[index] ) ) {
        list_add( moves, move_new( index, index + 8, piece_type ) );
        // list_add( &moves, move_new( index, index + 16, piece_type ) );
    } else {
        list_add( moves, move_new( index, index - 8, piece_type ) );
        // list_add( &moves, move_new( index, index - 16, piece_type ) );
    }
}

void get_rook_moves( chess_board *cb, int index, List **moves ) {
    int piece_type = get_type( get_piece( cb, index ) );
    int new_loc;
    // Go left
    
    for ( char i = get_col(index)-1; i >= 'A'; --i ) {
        new_loc = get_index( i, get_row( index ) );
        if ( cb->board[new_loc] == 0 )
            list_add( moves, move_new( index, new_loc, piece_type ) );
        else {
            if ( is_white( get_piece( cb, new_loc ) ) != is_white( get_piece( cb, index ) ) )
                list_add( moves, move_new( index, new_loc, piece_type ) );
            break;
        }
        
    }
    // Go right
    for ( char i = get_col(index)+1; i <= 'H'; ++i ) {
        new_loc = get_index( i, get_row( index ) );
        // printf( "New loc %d\n", new_loc );
        // printf( "For char %c\n", i );
        // printf( "For row %d\n", get_row( index ));
        // printf( "For col %d\n", get_col(index));
        if ( cb->board[new_loc] == 0 )
            list_add( moves, move_new( index, new_loc, piece_type ) );
        else {
            if ( is_white( get_piece( cb, new_loc ) ) != is_white( get_piece( cb, index ) ) )
                list_add( moves, move_new( index, new_loc, piece_type ) );
            break;
        }
    }
    // Go up
    for ( int i = get_row(index)+1; i < 8; ++i ) {
        new_loc = get_index( get_col(index), i );
        // printf("new loc %d\n", new_loc);
        if ( cb->board[new_loc] == 0 )
            list_add( moves, move_new( index, new_loc, piece_type ) );
        else {
            if ( is_white( get_piece( cb, new_loc ) ) != is_white( get_piece( cb, index ) ) )
                list_add( moves, move_new( index, new_loc, piece_type ) );
            break;
        }
    }
    // Go down
    for ( int i = get_row(index)-1; i >= 0; ++i ) {
        new_loc = get_index( get_col(index), i );
        if ( cb->board[new_loc] == 0 )
            list_add( moves, move_new( index, new_loc, piece_type ) );
        else {
            if ( is_white( get_piece( cb, new_loc ) ) != is_white( get_piece( cb, index ) ) )
                list_add( moves, move_new( index, new_loc, piece_type ) );
            break;
        }
    }
}

void get_bishop_moves( chess_board *cb, int index, List **moves ) {
    int piece_type = get_type( get_piece( cb, index ) );
    // Go up-left
    for ( int i = index + 7; i < 64; i = i + 7 ) {
        if ( cb->board[i] == 0 )
            list_add( moves, move_new( index, i, piece_type ) );
        else {
            if ( is_white( get_piece( cb, i ) ) != is_white( get_piece( cb, index ) ) )
                list_add( moves, move_new( index, i, piece_type ) );
            break;
        }
                
    }
    // Go up-right
    for ( int i = index + 9; i < 64; i = i + 9 ) {
        if ( cb->board[i] == 0 )
            list_add( moves, move_new( index, i, piece_type ) );
        else {
            if ( is_white( get_piece( cb, i ) ) != is_white( get_piece( cb, index ) ) )
                list_add( moves, move_new( index, i, piece_type ) );
            break;
        }
    }
    // Go down-left
    for ( int i = index - 9; i >= 0; i = i - 9 ) {
        if ( cb->board[i] == 0 )
            list_add( moves, move_new( index, i, piece_type ) );
        else {
            if ( is_white( get_piece( cb, i ) ) != is_white( get_piece( cb, index ) ) )
                list_add( moves, move_new( index, i, piece_type ) );
            break;
        }
    }
    // Go down-right
    for ( int i = index - 7; i >= 0; i = i - 7 ) {
        if ( cb->board[i] == 0 )
            list_add( moves, move_new( index, i, piece_type ) );
        else {
            if ( is_white( get_piece( cb, i ) ) != is_white( get_piece( cb, index ) ) )
                list_add( moves, move_new( index, i, piece_type ) );
            break;
        }
    }
}

void get_knight_moves( chess_board *cb, int index, List **moves ) {
    int piece_type = get_type( get_piece( cb, index ) );

    int new_pos;

    new_pos = index + 6;
    if ( new_pos < 64 && get_col( index ) < 'F' && get_col( index ) > 'B' ) {
        // Check if any piece is there.
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index + 15;
    if ( new_pos < 64 ) {
        // Check if any piece is there.
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index + 17;
    if ( new_pos < 64 ) {
        // Check if any piece is there.
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index + 10;
    if ( new_pos < 64 && get_col( index ) < 'F' && get_col( index ) > 'B' ) { // Ignore if we are located at g or h
        // Check if any piece is there.
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index - 6;
    if ( new_pos >= 0 && get_col( index ) < 'F' && get_col( index ) > 'B' ) {
        // Check if any piece is there.
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index - 10;
    if ( new_pos >= 0 && get_col( index ) < 'F' && get_col( index ) > 'B' ) {
        // Check if any piece is there.
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index - 15;
    if ( new_pos >= 0 ) {
        // Check if any piece is there.
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index - 17;
    if ( new_pos >= 0 ) {
        // Check if any piece is there.
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
}

void get_queen_moves( chess_board *cb, int index, List **moves ) {
    get_rook_moves( cb, index, moves );
    get_bishop_moves( cb, index, moves );
}

void get_king_moves( chess_board *cb, int index, List **moves ) {
    int piece_type = get_type( get_piece( cb, index ) );

    int new_pos;

    new_pos = index + 1;
    if ( new_pos < 64 ) {
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
        
    new_pos = index + 7;
    if ( new_pos < 64 ) {
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index + 8;
    if ( new_pos < 64 ) {
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index + 9;
    if ( new_pos < 64 ) {
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index - 1;
    if ( new_pos >= 0 ) {
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index - 7;
    if ( new_pos >= 0 ) {
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index - 8;
    if ( new_pos >= 0 ) {
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
    new_pos = index - 9;
    if ( new_pos >= 0 ) {
        if ( cb->board[new_pos] == 0 )
            list_add( moves, move_new( index, new_pos, piece_type ) );
        else if ( is_white( get_piece( cb, new_pos ) ) != is_white( get_piece( cb, index ) ) )
            list_add( moves, move_new( index, new_pos, piece_type ) );
    }
}