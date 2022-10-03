#ifndef UTILS
#define UTILS
#include "fsstructure.h"
/**
 * @brief This function returns an array of char * computed from splitting the given str each time a matchinc character c is seen.
 * 
 * @param c The character to match
 * @param str The string to split
 * @return char** The array of char *
 */
char **str_split( const char c, const char *str );



/**
 * @brief This function counts the occurrences of c in str
 * 
 * @param c The character to check for
 * @param str The string to check the character in
 * @return int The amount of occorrences of c in str
 */
int str_count( const char c, const char *str );

/**
 * @brief This function deallocates an array of strings.
 * 
 * @param s The array of strings.
 * @param size The length of the array.
 */
void str_delete_split( char **s, int size );

/**
 * @brief This function returns the parent fsstruct specified in the path.
 * 
 * @param path The path to the file
 * @param root The root of the filesystem
 * @return fsstruct* The parent of the file
 */
fsstruct *locate_parent_folder( const char *path, fsstruct *root );

/**
 * @brief This function returns the folder/file at the given path.Returns NULL if the file was not found.
 * 
 * @param path The path to the folder
 * @param root The root of the filesystem
 * @return fsstruct* The folder/file
 */
fsstruct *locate_folder_file( const char *path, fsstruct *root );

/**
 * @brief This function returns the file in the given folder. Returns NULL if the file was not found.
 * 
 * @param filename The name of the file
 * @param folder The folder containing the file
 * @return fsstruct* The file
 */
fsstruct *locate_file( const char *filename, fsstruct *folder ); // NOT USED 


/**
 * @brief This function copies size bytes into the given buffer at the given offset.
 * Precondition: The buffer's size should be at least as big as the given size.
 * @param file The file
 * @param buffer The buffer to copy into
 * @param size The amount of bytes to read
 * @param offset The offset into the file
 * @return int The amount of bytes read.
 */
int file_read( fsstruct *file, char *buffer, size_t size, off_t offset );

/**
 * @brief This function writes size bytes into the given file from the buffer starting at the given offset in the file.
 * 
 * @param file The file
 * @param buffer The buffer to write
 * @param size The amount of bytes to write
 * @param offset The offset at which to 'offset' the write inside the file
 * @return int The amount of bytes written
 */
int file_write( fsstruct *file, const char *buffer, size_t size, off_t offset );

/**
 * @brief This function reads from the given file the filesystem. If the file doesn't exist, an empty filesystem will be created.
 * 
 * @param file The file containing the datastructure / filesystem
 * @return fsstruct* The root of the filesystem / datastructure
 */
fsstruct *disk_read( char *file );

/**
 * @brief This function writes the entire filesystem to disk
 * 
 * @param file The file for storing the data.
 * @param root The root of the filestructure.
 * @return int Success code. Error: -1.
 */
int disk_write( char *file, fsstruct *root );

/**
 * @brief This function deallocates the filesystem. It recursivly deallocates the folder's subfolders and files.
 * 
 * @param root The root of the filesystem.
 */
void fsdealloc( fsstruct *root );


#endif