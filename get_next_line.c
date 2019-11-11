#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "get_next_line.h"

size_t      ft_strsizeof(char *str)
{
    return (sizeof(char) * strlen(str));
}

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

size_t		scan_buff(char *buffer)
{
    size_t		i;

    i = 0;
    while (i < BUFF_SIZE && buffer[i])
    {
        if (buffer[i] == '\n')
            break ;
        i++;
    }
    return (i);
}

int         get_next_line(const int fd, char **line)
{
    char            buffer[BUFF_SIZE];
    static char     prev_buffer[BUFF_SIZE];
    static size_t   prev_line_len;
    size_t          line_len;
    size_t          tmp;

    line_len = BUFF_SIZE;
    if (strlen(prev_buffer) > 0)
    {
        tmp = prev_line_len;
        prev_line_len = scan_buff(&prev_buffer[prev_line_len]);
        line[0] = (char*)malloc(sizeof(char) * prev_line_len + 1);
        line[0][prev_line_len] = '\0';
        if (prev_line_len > 0)
            strncpy(line[0], &prev_buffer[tmp], prev_line_len);
        prev_line_len += tmp + 1;
        if (prev_buffer[prev_line_len - 1] == '\n')
        {
            printf("%s\n", line[0]);
            return (1);
        }
        prev_line_len = 0;
        tmp = 1;
        memset(prev_buffer, '\0', BUFF_SIZE);
    }
    while (line_len == BUFF_SIZE)
    {
        tmp = read(fd, buffer, BUFF_SIZE);
        line_len = scan_buff(buffer);
        if (line[0] == NULL)
        {
            line[0] = (char*)malloc(sizeof(char) * line_len + 1);
            line[0][line_len] = '\0';
            strncpy(line[0], buffer, line_len);
        }
        else
        {
            line[0] = ft_strrealloc(line[0], ft_strsizeof(line[0]) + line_len);
            line[0][strlen(line[0]) + line_len] = '\0';
            strncpy(&line[0][strlen(line[0])], buffer, line_len);
        }
        if (line_len != BUFF_SIZE)
        {
            if (buffer[line_len] == '\n')
                line_len++;
            if (line_len > 0)
                strncpy(prev_buffer, &buffer[line_len], BUFF_SIZE - line_len);
            prev_buffer[BUFF_SIZE - line_len] = '\0';
        }
        memset(buffer, '\0', BUFF_SIZE);
    }
    if (tmp == 0 && line_len == 0)
        return (0);
    printf("%s\n", line[0]);
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
//     get_next_line(fd, line);
//     line++;
//     get_next_line(fd, line);
//     line++;
//     get_next_line(fd, line);
//     line++;
//     get_next_line(fd, line);
//     // line++;
//     // get_next_line(fd, line);
//     // line++;
//     // get_next_line(fd, line);
//     close(fd);
//     return (1);
// }