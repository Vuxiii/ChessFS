#ifndef EASTEREGG
#define EASTEREGG
#include <fuse.h>
#include "fsstructure.h"


int easter_readdir( const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, fsstruct *root );


#endif