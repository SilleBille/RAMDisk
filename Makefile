
CC=gcc
CFLAGS= -g $(PKGFLAGS) -lm
PKGFLAGS=`pkg-config fuse --cflags --libs`
OBJ= ramdisk.o ramnode.o

all: ramdisk

%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@ 
        
ramdisk: $(OBJ)
	$(CC) $^ $(CFLAGS) -o $@ 


clean:
	rm *.o ramdisk