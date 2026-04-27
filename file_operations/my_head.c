/************************************************
 * PROGRAM       : my_head.c
 *
 * FUNCTIONALITY :
 * Displays first 10 lines of a file
 * similar to Linux `head` command.
 *
 * SYSTEM CALLS  :
 * open(), read(), write(), close()
 *
 * USAGE         :
 * ./my_head file.txt
 ************************************************/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd, ret, lines = 0;
    char ch;

    if (argc < 2)
    {
        write(2, "Usage: ./my_head <file>\n", 25);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while ((ret = read(fd, &ch, 1)) > 0 && lines < 10)
    {
        write(1, &ch, 1);

        if (ch == '\n')
            lines++;
    }

    close(fd);
    return 0;
}