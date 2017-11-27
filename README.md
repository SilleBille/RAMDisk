P4-Ramdisk
==========

Implements a dictionary and stores files inside the RAM. 

Also, dumps all files and folders to persistent storage if a file name is given

Instructions to run the FS
--------------------------

    make clean
    make

    ./ramdisk < mount point > < size in MB > [< file name >]

Here `< mount point >` and `< size in MB >` are mandatory. `<file name>` is the file where the files and folders in the RAMdisk are dumped into and read when remounted.
