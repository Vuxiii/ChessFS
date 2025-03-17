# ChessFS
To mount the filesystem, change the script `m.sh` and `um.sh`, so it points to the desired mountpoint.
`m.sh` mounts the filesystem
`um.sh` unmounts the filesystem

Note: To make sure that the filesystem works, FUSE needs to be installed. 

To play chess make a folder called `chess` and enter it. A file called board will be generated with the current chess board in ascii-art.
All valid moves will be represented by a folder. Change directory into a folder to make a move.
