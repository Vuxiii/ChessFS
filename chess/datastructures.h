#ifndef DATASTRUCTURES
#define DATASTRUCTURES

#include <stddef.h>

typedef struct list List;
typedef struct move Move;

struct list {
    void *data;
    struct list *next;
};

struct move {
    int from;
    int to;
    int move_type;
};

#define LISTSIZE sizeof( List )
#define MOVESIZE sizeof( Move )


/**
 * @brief This function creates a new Move.
 * 
 * @param from The index on the board the piece is moving from
 * @param to The index on the board the piece is moving to
 * @param move_type The type of the move.
 * @return Move* The created move.
 */
Move *move_new( int from, int to, int move_type );

/**
 * @brief This function deallocates the move
 * 
 * @param move The move to deallocate
 */
void move_delete( Move *move );

/**
 * @brief This function creates a new linked-list.
 * 
 * @param data The data to be stored.
 * @return list* The linked list
 */
List *list_new( void *data );

/**
 * @brief This function deallocates the list structure.
 * 
 * @param free_function The function to call on each data entry to free it.
 * @param list The list to deallocate.
 */
void list_delete( List *list, void (*free_function)( void * ) );

/**
 * @brief This function adds the given data to the Linked-List
 * 
 * @param list The list
 * @param data The data to add
 * @return List The head of the list.
 */
List *list_add( List **list, void *data );

/**
 * @brief This function calls the given function passing each data entry in the list
 * 
 * @param list The list to run over
 * @param func The function to call on each element in the list
 */
void list_foreach( List *list, void (*func)( void * ) );

/**
 * @brief This function calls the given function passing each data entry in the list
 * 
 * @param list The list to run over
 * @param func The function to call on each element in the list
 * @param data1 Data field to be passed along as second argument of func.
 */
void list_foreach_with_data1( List *list, void (*func)( void *, void * ), void *data1 );

/**
 * @brief This function calls the given function passing each data entry in the list
 * 
 * @param list The list to run over
 * @param func The function to call on each element in the list
 * @param data1 Data field to be passed along as second argument of func.
 * @param data2 Data field to be passed along as third argument of func.
 */
void list_foreach_with_data2( List *list, void (*func)( void *, void *, void * ), void *data1, void *data2 );

/**
 * @brief This function calls the given function passing each data entry in the list
 * 
 * @param list The list to run over
 * @param func The function to call on each element in the list
 * @param data1 Data field to be passed along as second argument of func.
 * @param data2 Data field to be passed along as third argument of func.
 * @param data3 Data field to be passed along as fourth argument of func.
 */
void list_foreach_with_data3( List *list, void (*func)( void *, void *, void *, void * ), void *data1, void *data2, void *data3 );

/**
 * @brief This function returns the first element in the list.
 * 
 * @param list The list
 * @return void* The data contained in the first element
 */
void *list_first( List *list );

/**
 * @brief This function returns the last element in the list
 * 
 * @param list The list
 * @return void* The data contained in the last element
 */
void *list_last( List *list );

/**
 * @brief This function return the size of the list.
 * 
 * @param list The list
 * @return size_t The size of the list.
 */
size_t list_size( List *list );

#endif