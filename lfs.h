#ifndef LFSH
#define LDSH

int lfs_getattr      ( const char *, struct stat * );
int lfs_readdir      ( const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info * );
int lfs_open         ( const char *, struct fuse_file_info * );
int lfs_read         ( const char *, char *, size_t, off_t, struct fuse_file_info * );
int lfs_write        ( const char *, const char *, size_t, off_t, struct fuse_file_info * );
int lfs_release      ( const char *, struct fuse_file_info * );
int lfs_make_file    ( const char *, mode_t, dev_t );
int lfs_utimens      ( const char *, struct utimbuf * );
int lfs_make_dir     ( const char *, mode_t );
int lfs_remove_file  ( const char * );
int lfs_remove_folder( const char * );
int lfs_truncate     ( const char *, off_t );
int lfs_rename       ( const char *, const char* );

#endif