/************************************************
 * PROGRAM       : wc.c
 *
 * FUNCTIONALITY :
 * Counts lines, words, and characters
 * similar to Linux `wc` command.
 *
 * OPTIONS       :
 * -l → lines
 * -w → words
 * -c → characters
 ************************************************/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd, ret;
    char ch;

    int lines = 0, words = 0, chars = 0;
    int in_word = 0;

    int flag_l = 0, flag_w = 0, flag_c = 0;
    char *filename = NULL;

    if (argc < 2)
    {
        write(2, "Usage: wc [-lwc] file\n", 22);
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-l") == 0) flag_l = 1;
        else if (strcmp(argv[i], "-w") == 0) flag_w = 1;
        else if (strcmp(argv[i], "-c") == 0) flag_c = 1;
        else filename = argv[i];
    }

    if (!filename)
    {
        write(2, "File missing\n", 13);
        return 1;
    }

    if (!flag_l && !flag_w && !flag_c)
        flag_l = flag_w = flag_c = 1;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    while ((ret = read(fd, &ch, 1)) > 0)
    {
        chars++;

        if (ch == '\n') lines++;

        if (ch == ' ' || ch == '\n' || ch == '\t')
            in_word = 0;
        else if (!in_word)
        {
            words++;
            in_word = 1;
        }
    }

    close(fd);

    if (flag_l) printf("%d ", lines);
    if (flag_w) printf("%d ", words);
    if (flag_c) printf("%d ", chars);

    printf("%s\n", filename);

    return 0;
}