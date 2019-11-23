#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "get_next_line.h"

char        *ft_strrealloc(char *str, size_t needed_memory)
{
    void    *str_cpy;

    if (needed_memory == 0)
        return (str);
    str_cpy = (char*)malloc(sizeof(char) * sizeof(str));
    strcpy(str_cpy, str);
    free(str);
    str = (char*)malloc(sizeof(char) * (needed_memory + strlen(str_cpy) + 1));
    strcpy(str, str_cpy);
    str[needed_memory + strlen(str_cpy)] = '\0';
    free(str_cpy);
    return (str);
}

int     check_res(char **line, char *res)
{
    static char     *new_line_pos = NULL;
    char            *buffer;
    
    if (new_line_pos != NULL)
        buffer = new_line_pos;
    else
        buffer = res;
    if ((new_line_pos = strchr(buffer, '\n')))
    {
        line[0] = (char*)malloc(sizeof(char) * (new_line_pos - buffer) + 1);
        strncpy(line[0], buffer, new_line_pos - buffer);
        line[0][new_line_pos - buffer] = '\0';
        new_line_pos++;
        printf("%d", 1);
        return (1);
    }
    else
    {
        line[0] = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
        strncpy(line[0], buffer, strlen(buffer));
        line[0][strlen(buffer)] = '\0';
        printf("%d", 0);
        return (0);
    }
}

int         get_next_line(const int fd, char **line)
{
    static char res[BUFF_SIZE];
    char        buffer[BUFF_SIZE];
    int         read_result;
    char        *new_line_pos;

    if (fd < 0 || fd > MAX_FD)
        return (-1);
    if (line[0] != NULL)
        free(line[0]);
    line[0] = (char*)malloc(sizeof(char));
    line[0][0] = '\0';
    if (check_res(line, res))
        return (1);
    read_result = read(fd, buffer, BUFF_SIZE);
    if (read_result == 0)
    {
        // if (strlen(res) > 0)
        //     return (1);
        return (0);
    }
    while ((new_line_pos = strchr(buffer, '\n')) == NULL)
    {
        line[0] = ft_strrealloc(line[0], (size_t)read_result);
        strncpy(&line[0][strlen(line[0])], buffer, read_result);
        read_result = read(fd, buffer, BUFF_SIZE);
        if (read_result == 0)
            return (0);
    }
    line[0] = ft_strrealloc(line[0], (new_line_pos - buffer));
    strncpy(&line[0][strlen(line[0])], buffer, new_line_pos - buffer);
    strncpy(res, &buffer[new_line_pos - buffer + 1], read_result - (new_line_pos - buffer));
    res[read_result - (new_line_pos - buffer) - 1] = '\0';
    return (1);
}

// int		main(int argc, char **argv)
// {
//     int		state;
//     int		fd;
//     char	**line;

//     state = 1;
//     fd = open(argv[1], O_RDONLY);
//     line = (char**)malloc(sizeof(char*) * 100);
//     printf("\n%d", get_next_line(fd, line));
//     printf("\n%s", *line);   
//     line++;
//     printf("\n%d", get_next_line(fd, line));
//     printf("\n%s", *line);
//     line++;
//     printf("\n%d", get_next_line(fd, line));
//     printf("\n%s", *line);
//     line++;
//     printf("\n%d", get_next_line(fd, line));
//     printf("\n%s", *line);
// }