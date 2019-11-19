#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "get_next_line.h"

char        *ft_strrealloc(char *str, size_t new_memory_size)
{
    void    *str_cpy;

    if (new_memory_size == 0)
        return (str);
    str_cpy = (char*)malloc(sizeof(char) * sizeof(str));
    strcpy(str_cpy, str);
    free(str);
    str = (char*)malloc(sizeof(char) * new_memory_size);
    strcpy(str, str_cpy);
    free(str_cpy);
    return (str);
}

int         get_next_line(const int fd, char **line)
{
    char        buffer[BUFF_SIZE];
    static char res[BUFF_SIZE];
    int         read_result;
    char        *new_line_pos;

    new_line_pos = NULL;
    *line = malloc(sizeof(char));
    *line[0] = '\0';
    read_result = read(fd, buffer, BUFF_SIZE);
    while ((new_line_pos = strchr(buffer, '\n')) == NULL)
    {
        ft_strrealloc(*line, strlen(*line) + BUFF_SIZE + 1);
        strncpy(line[strlen(*line)], buffer, BUFF_SIZE);
        printf("%lu", strlen(*line));
        if (read_result == 0)
            return (0);
        read_result = read(fd, buffer, BUFF_SIZE);
    }
    ft_strrealloc(*line, strlen(*line) + BUFF_SIZE - (new_line_pos - buffer) + 1);
    strncpy(line[strlen(*line)], buffer, new_line_pos - buffer);
    strncpy(res, &buffer[new_line_pos - buffer], BUFF_SIZE - (new_line_pos - buffer));
    printf("%s", *line);
    return (1);
}

int		main(int argc, char **argv)
{
    int		state;
    int		fd;
    char	**line;

    state = 1;
    fd = open(argv[1], O_RDONLY);
    line = (char**)malloc(sizeof(char*) * 100);
    // printf("%d", get_next_line(fd, line));
    get_next_line(fd, line);
}