#ifndef MOVES
#define MOVES

#include "chess.h"
#include "datastructures.h"

/**
 * @brief This function prints a move to the screen
 * 
 * @param move The move to be printed.
 */
void print_move( Move *move );

/**
 * @brief This function returns a List containing all moves for the given board.
 * 
 * @param cb The pointer to the chess_board struct
 * @return List* The list containing all the Moves.
 */
List *get_all_moves( chess_board *cb );

/**
 * @brief This function returns a list containing all the moves the given piece located at index can make.
 * 
 * @param pi The pointer to the information.
 * @param cb The pointer to the chess board struct
 * @param moves The list where moves will be added.
 */
void get_moves_for_index( piece_info *pi, chess_board *cb, List **moves );

/**
 * @brief Returns the moves for the given pawn.
 * 
 * @param cb The pointer to the chess board
 * @param index The index to locate the pawn.
 */
void get_pawn_moves( chess_board *cb, int index, List **moves );

/**
 * @brief Returns the moves for the given rook.
 * 
 * @param cb The pointer to the chess board
 * @param index The index to locate the rook.
 */
void get_rook_moves( chess_board *cb, int index, List **moves );

/**
 * @brief Returns the moves for the given bishop.
 * 
 * @param cb The pointer to the chess board
 * @param index The index to locate the bishop.
 */
void get_bishop_moves( chess_board *cb, int index, List **moves );

/**
 * @brief Returns the moves for the given knight.
 * 
 * @param cb The pointer to the chess board
 * @param index The index to locate the knight.
 */
void get_knight_moves( chess_board *cb, int index, List **moves );

/**
 * @brief Returns the moves for the given queen.
 * 
 * @param cb The pointer to the chess board
 * @param index The index to locate the queen.
 */
void get_queen_moves( chess_board *cb, int index, List **moves );

/**
 * @brief Returns the moves for the given king.
 * 
 * @param cb The pointer to the chess board
 * @param index The index to locate the king.
 */
void get_king_moves( chess_board *cb, int index, List **moves );
#endif