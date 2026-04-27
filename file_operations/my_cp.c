/************************************************
 * PROGRAM       : my_cp.c
 *
 * FUNCTIONALITY :
 * Copies contents of one file to another file
 * similar to Linux `cp` command.
 *
 * DESCRIPTION   :
 * Uses system calls (open, read, write, close)
 * to copy data from source file to destination.
 *
 * SYSTEM CALLS  :
 * open(), read(), write(), close()
 *
 * USAGE         :
 * ./my_cp source.txt dest.txt
 *
 * NOTES         :
 * - Creates destination file if not present
 * - Overwrites existing file
 *
 * AUTHOR        : Sourabh B Sthavaramath
 ************************************************/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int fd1, fd2, ret;
    char buffer[BUF_SIZE];

    if (argc != 3)
    {
        write(2, "Usage: ./my_cp <src> <dest>\n", 28);
        exit(EXIT_FAILURE);
    }

    fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0)
    {
        perror("open source");
        exit(EXIT_FAILURE);
    }

    fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 < 0)
    {
        perror("open dest");
        close(fd1);
        exit(EXIT_FAILURE);
    }

    while ((ret = read(fd1, buffer, BUF_SIZE)) > 0)
    {
        if (write(fd2, buffer, ret) != ret)
        {
            perror("write");
            close(fd1);
            close(fd2);
            exit(EXIT_FAILURE);
        }
    }

    if (ret < 0)
        perror("read");

    close(fd1);
    close(fd2);

    return 0;
}