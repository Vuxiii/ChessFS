#ifndef MAINCHESS
#define MAINCHESS

#include <string.h>
#include <wchar.h>
#include "datastructures.h"

typedef struct chess_board chess_board;
typedef struct piece_info piece_info;
typedef char chess_piece;

#define PAWN    1 
#define ROOK    2 
#define BISHOP  3 
#define KNIGHT  4 
#define QUEEN   5 
#define KING    6 

#define COLOR   1 << 3
#define BLACK   0
#define WHITE   1



// 000 000000

// Pawn     001
// Rook     010
// Bishop   011
// Knight   100
// Queen    101
// King     110

// Black 0
// White 1

// Pos 000 000

// chess_Piece = [COLOR | TYPE] -> [1 bit | 3 bit]

struct chess_board {
    chess_piece *board;
    int is_white;
};

struct piece_info {
    chess_piece piece;
    int loc;
};

#define CBSIZE sizeof(chess_board)
#define CBENTRIESSIZE sizeof( chess_piece ) * 64


/**
 * @brief This function returns a string representation of the current board.
 * 
 * @param cb The pointer to the chess board.
 * @return char The string representation of the board.
 */
char *chess_to_ascii( chess_board *cb );

chess_board *chess_new_board();

/**
 * @brief This function returns a new chess board from the given path.
 * 
 * @param path Moves delimited with a '/'. "B1A3/B7B6"
 * @return chess_board* The resulting chess board found by parsing the given moves.
 */
chess_board *chess_new_board_from_path( char *path );

/**
 * @brief This function berforms the given move.
 * 
 * @param cb The chess board
 * @param move The move to make
 */
void make_move( chess_board *cb, Move *move );

/**
 * @brief This function returns the piece constructed from the given color and desired piece.
 * 
 * @param color The color of the piece
 * @param piece The actual piece itself
 * @return piece [1 bit | 3 bit] -> [COLOR|PIECE]
 */
chess_piece create_piece( int color, int piece );


/**
 * @brief This function creates an entry combining the piece and its location
 * 
 * @param piece The piece
 * @param index The piece's location on the board
 * @return piece_info* The struct storing the above.
 */
piece_info *create_piece_entry( chess_piece piece, int index );

/**
 * @brief This function deallocates the struct pi
 * 
 * @param pi The struct piece_info to be deallocated
 */
void delete_piece_entry( piece_info *pi );

/**
 * @brief This function collects all the pieces on the board
 * 
 * @param cb The pointer to the chess_board
 * @return List* A List containing all the pieces and their locations. Each entry is a struct piece_info
 */
List *collect_all_pieces( chess_board *cb );



/**
 * @brief This function returns the piece located at the given index
 * 
 * @param cb The chess_board struct
 * @param index The index
 * @return piece The piece stored at the index on the board.
 */
chess_piece get_piece( chess_board *cb, int index );

/**
 * @brief This function returns the type of the given piece.
 * 
 * @param piece The piece
 * @return int The type of the piece.
 */
int get_type( chess_piece piece );

/**
 * @brief This function Checks whehter the given piece is white
 * 
 * @param piece The piece to check
 * @return int 0 if the piece is white.
 */
int is_white( chess_piece piece );


/**
 * @brief This function calculates the index based on the given row and column.
 * 
 * @param col ['A'..'H']
 * @param row [1..8]
 * @return int The index on the board.
 */
int get_index( char col, int row );

/**
 * @brief This function returns the row of the given index.
 * 
 * @param index The index on the board
 * @return int [1..8]
 */
int get_row( int index );

/**
 * @brief This function returns the column of the given index. 
 * 
 * @param index The index on the board
 * @return char ['A'..'H']
 */
char get_col( int index );

/**
 * @brief This function prints the piece to the screen
 * 
 * @param piece The piece to be printed.
 */
void print_piece( chess_piece piece );

/**
 * @brief This function checks whehter the given bit is set in piece
 * 
 * @param piece The piece to check for the set bit
 * @param bit The bit to check
 * @return int 0 if not set.
 */
int check_set( chess_piece piece, unsigned int bit );

#endif