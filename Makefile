GCC = gcc
SOURCES = utils.c fsstructure.c lfs.c easteregg.c chess/chess.c chess/datastructures.c chess/moves.c
OBJS := $(patsubst %.c,%.o,$(SOURCES))
# OBJS := 
CFLAGS = -O2 -Wall -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=25 -g -Wall -Wextra

.PHONY: lfs

##
# Libs 
##
LIBS := fuse 
LIBS := $(addprefix -l,$(LIBS))

all: lfs

# chess.o: moves.o datastructures.o
# moves.o: 
# datastructures.o: 

%.o: %.c
	$(GCC) $(CFLAGS) -c -o $@ $<

lfs: $(OBJS) 
	$(GCC) $(OBJS) $(LIBS) $(CFLAGS) -o lfs

clean:
	rm -f $(OBJS) lfs fsstructure.o utils.o
