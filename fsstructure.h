#ifndef FSSTRUCT
#define FSSTRUCT
// #include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>

#define FOLDER_T 1
#define FILE_T   2

typedef int fs_t;

typedef struct fsstruct {
    char *name;                     // FILE & FOLDER:   The name of the file/folder.
    char *data;                     // FILE ONLY:       The data stored.
    size_t name_size;               // FILE & FOLDER:   The length of the name in bytes.
    size_t size;                    // FILE ONLY:       The length of the stored data in bytes.
    size_t max_size;                // FILE ONLY:       The amount of memory allocated for the buffer.
    struct fsstruct *(*subdirs);    // FOLDER ONLY:     Any folders/files contained inside this folder.
    int type;                       // FILE & FOLDER:   FILE -> FILE_T | FOLDER -> FOLDER_T
    unsigned long id;               // FILE & FOLDER:   A unique ID for this entity.
    unsigned long entries;          // FOLDER ONLY:     The amount of files/folders contained inside this folder.
    unsigned long max_entries;      // FOLDER ONLY:     The maximum amount of folders currently available inside this folder. Functions like arraylist.
    time_t last_access_time;        // FILE & FOLDER:   This holds the last access time.
    time_t last_modification_time;  // FILE & FOLDER:   This holds the last modification time.
} fsstruct;

#define FSSTSIZE sizeof(fsstruct)
#define MAX_ENTRIES 10
#define FILE_DEFAULT_SIZE 4096 // Bytes
/**
 * @brief This function creates a new file.
 * 
 * @param f_name the name of the file
 * @return fsstruct* 
 */
fsstruct *fs_new_file( char *f_name );

/**
 * @brief This function deallocates the given file.
 * 
 * @param file The file to remove.
 */
void fs_rm_file( fsstruct *file );

/**
 * @brief This function creates a new folder.
 * 
 * @param f_name the name of the folder
 * @return fsstruct* 
 */
fsstruct *fs_new_folder( char *f_name );

/**
 * @brief This function deallocates the given folder.
 * 
 * @param folder The folder to remove.
 */
void fs_rm_folder( fsstruct *folder );

/**
 * @brief This function adds the given structure to the given folder struct.
 * 
 * @param folder The folder to add the node to
 * @param node The node to be inserted into the given folder
 * 
 * @return 0 for success.
 */
int fs_add_fsstruct_to_folder( fsstruct *folder, fsstruct *node );

/**
 * @brief This function writes the given file to the file at the given offset.
 * 
 * @param fd The filedescriptor
 * @param file The file to be written
 * @return size_t The amount of bytes written to the file descriptor.
 */
size_t disk_write_file( FILE *fd, fsstruct *file );

/**
 * @brief This function reads the given file and returns the datastructure for the filesystem
 * 
 * @param fd The file descriptor
 * @return fsstruct* The root of the filesystem.
 */
fsstruct *disk_read_file( FILE *fd );

/**
 * @brief This function writes the given folder to the file. It will also write each subdir contained in the folder
 * 
 * @param fd The file descriptor
 * @param folder The Folder
 * @return size_t Amount of bytes written.
 */
size_t disk_write_folder( FILE *fd, fsstruct *folder );


/**
 * @brief This function reads the folder contained in the file descriptor
 * 
 * @param fd The file descriptor
 * @return fsstruct* The folder
 */
fsstruct *disk_read_folder( FILE *fd );

/**
 * @brief This function returns a unique id.
 * 
 * @return unsigned long The unique id.
 */
unsigned long get_id();


#endif

