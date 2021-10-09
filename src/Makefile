CC=clang
INC_DIRS="."
CFLAGS=-std=c99 -g -Wall -Werror -O2 -I$(INC_DIRS)
LDFLAGS=-lpthread
OBJS=protocol.o \
     utils.o 		

all: oscdrp

%.o: %.c
	$(CC) $(CFLAGS) -c $^

oscdrp: main.c $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm $(OBJS) oscdrp
