#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <stdio.h> // delete me 
#include "datastructures.h"



Move *move_new( int from, int to, int move_type ) {
    Move *m = malloc( MOVESIZE );
    if ( m == NULL ) return NULL;
    
    m->from = from;
    m->to = to;
    m->move_type = move_type;

    return m;
}

void move_delete( Move *move ) {
    free( move );
}

List *list_new( void *data ) {
    List *list = malloc( LISTSIZE );
    if ( list == NULL ) return NULL;

    list->data = data;
    list->next = NULL;
    return list;
}

void list_delete( List *list, void (*free_function)( void * ) ) {
    // printf( "Called list delete\n" );
    List *current;
    List *next;
    // printf( "First it\n" );
    for ( current = list; current != NULL; current = next ) {
        
        next = current->next;
        if ( free_function != NULL ) { 
            // printf( "Calling free func\n" ); 
            free_function( current->data ); }
        // else printf( "For some reason the free function is null...\n" );
        free( current );
    }
    // printf( "First2 it\n" );
}

List *list_add( List **list, void *data ) {
    if ( *list == NULL ) {
        // printf("List is null\n");
        *list = list_new( data );
        if ( list == NULL ) return NULL;
    } else {
        // printf("not null\n");
        List *current = *list; 
        
        for ( List *next; current->next != NULL; current = next ) {
            next = current->next;
        }
        current->next = list_new( data );
        if ( current->next == NULL ) return NULL;
    }
    return *list;
}

void list_foreach( List *list, void (*func)( void * ) ) {
    for ( List *node = list; node != NULL; node = node->next ) {
        func( node->data );
    }
}

void list_foreach_with_data1( List *list, void (*func)( void *, void * ), void *data1 ) {
    for ( List *node = list; node != NULL; node = node->next ) {
        func( node->data, data1 );
    }
}

void list_foreach_with_data2( List *list, void (*func)( void *, void *, void * ), void *data1, void *data2 ) {
    for ( List *node = list; node != NULL; node = node->next ) {
        func( node->data, data1, data2 );
    }
}

void list_foreach_with_data3( List *list, void (*func)( void *, void *, void *, void * ), void *data1, void *data2, void *data3 ) {
    for ( List *node = list; node != NULL; node = node->next ) {
        func( node->data, data1, data2, data3 );
    }
}

void *list_first( List *list ) {
    if ( list == NULL ) return NULL;
    return list->data;
}

void *list_last( List *list ) {
    if ( list == NULL ) return NULL;
    
    List *current = list;
    
    for ( List *next = current->next; next != NULL; current = next ) {
        next = current->next;
    }
    return current->data;
}

size_t list_size( List *list ) {
    size_t size = 0;
    for ( List *current = list; current != NULL; current = current->next ) {
        size++;
    }
    return size;
}