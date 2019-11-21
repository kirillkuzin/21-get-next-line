#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "get_next_line.h"

char        *add_memory_to_start(char *str, size_t needed_memory)
{
    void    *str_cpy;

    if (needed_memory == 0)
        return (str);
    str_cpy = (char*)malloc(sizeof(char) * sizeof(str));
    strcpy(str_cpy, str);
    free(str);
    str = (char*)malloc(sizeof(char) * (needed_memory + strlen(str_cpy)));
    strcpy(&str[needed_memory], str_cpy);
    str[needed_memory + strlen(str_cpy)] = '\0';
    free(str_cpy);
    return (str);
}

int         free_res(int fd)
{
    return (1);
}

int         get_next_line(const int fd, char **line)
{
    char        buffer[BUFF_SIZE];
    static char res[BUFF_SIZE];
    int         read_result;
    int         result;
    char        *new_line_pos;

    read_result = read(fd, buffer, BUFF_SIZE);
    if ((new_line_pos = strchr(buffer, '\n')) == NULL)
    {
        result = get_next_line(fd, line);
        line[0] = add_memory_to_start(line[0], (size_t)read_result);
        strncpy(line[0], buffer, read_result);
    }
    else
    {
        line[0] = (char*)malloc(sizeof(char) * (new_line_pos - buffer) + 1);
        line[0][new_line_pos - buffer] = '\0';
        strncpy(line[0], buffer, new_line_pos - buffer);
        result = 1;
    }
    return (result);
}

int		main(int argc, char **argv)
{
    int		state;
    int		fd;
    char	**line;

    state = 1;
    fd = open(argv[1], O_RDONLY);
    line = (char**)malloc(sizeof(char*) * 100);
    printf("\n%d", get_next_line(fd, line));
    printf("\n%s", *line);
}