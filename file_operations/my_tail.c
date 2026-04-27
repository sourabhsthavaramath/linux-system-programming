/************************************************
 * PROGRAM       : my_tail.c
 *
 * FUNCTIONALITY :
 * Displays last 10 lines of a file
 * similar to Linux `tail` command.
 *
 * APPROACH      :
 * Count total lines → print last 10 lines
 ************************************************/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd, ret, total_lines = 0, current = 0;
    char ch;

    if (argc < 2)
    {
        write(2, "Usage: ./my_tail <file>\n", 25);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Count total lines
    while ((ret = read(fd, &ch, 1)) > 0)
    {
        if (ch == '\n')
            total_lines++;
    }

    close(fd);

    int target = total_lines - 10;
    if (target < 0) target = 0;

    fd = open(argv[1], O_RDONLY);

    while ((ret = read(fd, &ch, 1)) > 0)
    {
        if (ch == '\n')
            current++;

        if (current >= target)
            write(1, &ch, 1);
    }

    close(fd);
    return 0;
}