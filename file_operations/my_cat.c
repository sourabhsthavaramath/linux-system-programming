/************************************************
 * PROGRAM       : my_cat.c
 *
 * FUNCTIONALITY :
 * Displays the contents of one or more files
 * similar to the Linux `cat` command.
 *
 * DESCRIPTION   :
 * This program uses low-level system calls
 * (open, read, write, close) to read file data
 * and print it to standard output.
 *
 * SYSTEM CALLS  :
 * open(), read(), write(), close()
 *
 * USAGE         :
 * ./my_cat file1 file2 ...
 *
 * NOTES         :
 * - Uses buffered reading for efficiency
 * - Handles multiple files
 * - Prints error if file cannot be opened
 *
 * AUTHOR        : Sourabh B Sthavaramath
 ************************************************/

#include <unistd.h>   // read, write, close
#include <fcntl.h>    // open
#include <stdio.h>    // perror
#include <stdlib.h>   // exit

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int fd, ret, i;
    char buffer[BUF_SIZE];

    // Check for minimum arguments
    if (argc < 2)
    {
        write(2, "Usage: ./my_cat <file1> [file2 ...]\n", 37);
        exit(EXIT_FAILURE);
    }

    // Loop through all input files
    for (i = 1; i < argc; i++)
    {
        // Open file in read-only mode
        fd = open(argv[i], O_RDONLY);
        if (fd < 0)
        {
            perror("open");  // file not found / permission error
            continue;
        }

        // Read file in chunks and write to stdout
        while ((ret = read(fd, buffer, BUF_SIZE)) > 0)
        {
            if (write(1, buffer, ret) != ret)
            {
                perror("write");
                close(fd);
                exit(EXIT_FAILURE);
            }
        }

        // Check for read error
        if (ret < 0)
        {
            perror("read");
        }

        // Close file
        close(fd);
    }

    return 0;
}