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

char        *ft_strrealloc(char *str, size_t needed_memory)
{
    void    *str_cpy;

    if (needed_memory == 0)
        return (str);
    str_cpy = (char*)malloc(sizeof(char) * sizeof(str));
    strcpy(str_cpy, str);
    free(str);
    str = (char*)malloc(sizeof(char) * (needed_memory + strlen(str_cpy)));
    strcpy(str, str_cpy);
    free(str_cpy);
    return (str);
}

int        check_res(int fd, char **line, char *res)
{
    static char         *new_line_pos;
    char                *saved;

    line[0] = NULL;
    saved = new_line_pos;
    if (new_line_pos == NULL)
        new_line_pos = strchr(res, '\n');
    else
        new_line_pos = strchr(new_line_pos, '\n');
    if (new_line_pos == NULL && saved == NULL)
    {
        line[0] = (char*)malloc(sizeof(char) * strlen(res) + 1);
        strncpy(line[0], res, strlen(res));
        line[0][strlen(res)] = '\0';
    }
    else if (new_line_pos != NULL)
    {
        line[0] = (char*)malloc(sizeof(char) * (new_line_pos - res) + 1);
        line[0][new_line_pos - res] = '\0';
        strncpy(line[0], res, new_line_pos - res);
        new_line_pos++;
        return (1); 
    }
    else if (new_line_pos == NULL && saved != NULL)
    {
        line[0] = (char*)malloc(sizeof(char) * strlen(saved) + 1);
        line[0][strlen(saved)] = '\0';
        strcpy(line[0], saved);
    }
    return (0);
}

int         get_next_line(const int fd, char **line)
{
    static char res[BUFF_SIZE];
    char        buffer[BUFF_SIZE];
    int         read_result;
    int         result;
    char        *new_line_pos;

    if (check_res(fd, line, res))
        return (1);
    read_result = read(fd, buffer, BUFF_SIZE);
    if ((new_line_pos = strchr(buffer, '\n')) == NULL)
    {
        result = get_next_line(fd, line);
        line[0] = add_memory_to_start(line[0], (size_t)read_result);
        strncpy(line[0], buffer, read_result);
        // else
        // {
        //     line[0] = ft_strrealloc(line[0], (size_t)read_result);
        //     strncpy(&line[0][strlen(line[0])], buffer, read_result);
        // }
    }
    else
    {
        if (line[0] == NULL)
        {
            line[0] = (char*)malloc(sizeof(char) * (new_line_pos - buffer) + 1);
            line[0][new_line_pos - buffer] = '\0';
            strncpy(line[0], buffer, new_line_pos - buffer);
        }
        else
        {
            line[0] = ft_strrealloc(line[0], (new_line_pos - buffer) + 1);
            line[0][strlen(line[0]) + (new_line_pos - buffer)] = '\0';
            strncpy(&line[0][strlen(line[0])], buffer, new_line_pos - buffer);
        }
        strncpy(res, &buffer[new_line_pos - buffer + 1], read_result - (new_line_pos - buffer));
        res[read_result - (new_line_pos - buffer) - 1] = '\0';
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
    line++;
    printf("\n%d", get_next_line(fd, line));
    printf("\n%s", *line);
    line++;
    printf("\n%d", get_next_line(fd, line));
    printf("\n%s", *line);
}