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
    return (0);
}

int		get_next_line(const int fd, char **line)
{
    char	buffer[BUFF_SIZE];
    size_t	line_len;
    size_t	buff_count;

    buff_count = 0;
    read(fd, &buffer, BUFF_SIZE);
    line_len = scan_buff(&buffer[0]);
    while (line_len == 0)
    {
        buff_count++;
        read(fd, &buffer, BUFF_SIZE);
        line_len = scan_buff(&buffer[0]);
    }
    *line = (char*)malloc(sizeof(char) * (BUFF_SIZE * buff_count + line_len + 1));
    line[0][BUFF_SIZE * buff_count + line_len] = '\0';
    strncpy(line[0], &buffer[0], (size_t)(BUFF_SIZE * buff_count + line_len));
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
    while (state == 1)
        state = get_next_line(fd, line);
    close(fd);
    return (1);
}