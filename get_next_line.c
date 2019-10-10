/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggeordi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 21:17:26 by ggeordi           #+#    #+#             */
/*   Updated: 2019/10/05 16:03:16 by ggeordi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 6

size_t		scan_buff(char *buffer)
{
    size_t		i;

    i = 0;
    while (i < BUFF_SIZE && buffer[i])
    {
        if (buffer[i] == '\n')
            return (i);
        i++;
    }
    return (BUFF_SIZE + 1);
}

char    *ft_strrealloc(char *str, size_t new_memory_size)
{
    void    *str_cpy;

    str_cpy = (char*)malloc(sizeof(char) * sizeof(str));
    strcpy(str_cpy, str);
    free(str);
    str = (char*)malloc(sizeof(char) * new_memory_size);
    strcpy(str, str_cpy);
    free(str_cpy);
    return str;
}

size_t  ft_strsizeof(char *str)
{
    return (sizeof(char) * strlen(str));
}

int		get_next_line(const int fd, char **line)
{
    char    buffer[BUFF_SIZE + 1];
    static char    prev[BUFF_SIZE + 1];
    size_t	line_len;
    size_t	buff_count;
    size_t  a;

    buffer[BUFF_SIZE] = '\0';
    if (strlen(prev) > 0)
    {
        line[0] = (char*)malloc(ft_strsizeof(prev));
        strcpy(line[0], prev);
    }
    buff_count = 0;
    a = read(fd, &buffer, BUFF_SIZE);
    line_len = scan_buff(&buffer[0]);
    if (line_len == BUFF_SIZE + 1)
    {
        if (strlen(prev) > 0)
        {
            line[0] = ft_strrealloc(line[0], ft_strsizeof(line[0]) + BUFF_SIZE);
            strcpy(&line[0][strlen(prev)], &buffer[0]);
        }
        else
        {
            line[0] = (char*)malloc(sizeof(char) * BUFF_SIZE);
            strcpy(line[0], &buffer[0]);
        }
    }
    else
    {
        if (strlen(prev) > 0)
        {
            line[0] = ft_strrealloc(line[0], ft_strsizeof(line[0]) + line_len);
            strncpy(&line[0][strlen(prev)], &buffer[0], line_len);
        }
        else
        {
            line[0] = (char*)malloc(sizeof(char) * line_len);
            strncpy(line[0], &buffer[0], line_len);
        }
    }
//    memset(prev, '\0', BUFF_SIZE);
    if (a < BUFF_SIZE)
    {
        printf("%s", line[0]);
        return (0);
    }
    while (line_len == BUFF_SIZE + 1)
    {
        buff_count++;
        read(fd, &buffer, BUFF_SIZE);
        line_len = scan_buff(&buffer[0]);
        if (line_len == BUFF_SIZE + 1)
        {
            line[0] = ft_strrealloc(line[0], ft_strsizeof(line[0]) + BUFF_SIZE);
            strcpy(&line[0][buff_count * BUFF_SIZE], &buffer[0]);
        }
        else
        {
            line[0] = ft_strrealloc(line[0], ft_strsizeof(line[0]) + line_len);
            strncpy(&line[0][buff_count * BUFF_SIZE], &buffer[0], line_len);
            if (line_len != BUFF_SIZE)
            {
                strcpy(prev, &buffer[line_len]);
                prev[BUFF_SIZE - line_len] = '\0';
            }
        }
    }
//    line[0][BUFF_SIZE * buff_count + line_len] = '\0';
    printf("%s", line[0]);
    return (0);
}

int		main(int argc, char **argv)
{
    int		state;
    int		fd;
    char	**line;

    state = 1;
    fd = open(argv[1], O_RDONLY);
    line = (char**)malloc(sizeof(char*) * 100);
//    while (state == 1)
//        state = get_next_line(fd, line);
    get_next_line(fd, line);
    line++;
    get_next_line(fd, line);
//    line++;
//    get_next_line(fd, line);
//    line++;
//    get_next_line(fd, line);
    close(fd);
    return (1);
}