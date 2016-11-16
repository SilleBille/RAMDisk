
CC=gcc
CFLAGS= -g $(PKGFLAGS)
PKGFLAGS=`pkg-config fuse --cflags --libs`
OBJ= ramdisk.o

all: ramdisk

%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@ 
        
ramdisk: $(OBJ)
	$(CC) $^ $(CFLAGS) -o $@ 


clean:
	rm *.o ramdisk