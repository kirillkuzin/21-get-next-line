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

#define BUFF_SIZE 4096

int		get_next_line(const int fd, char **line)
{
	char    sym;
	size_t	line_len;
	size_t	buff_count;
	size_t  i;

	line_len = 0;
	i = 0;
    sym = 1;
	while (sym != '\n' && sym)
	{
		read(fd, &sym, 1);
		line_len++;
	}
	lseek(fd, -line_len, SEEK_CUR);
	*line = (char*)malloc(sizeof(char) * line_len);
	buff_count = line_len / BUFF_SIZE;
	if (line_len % BUFF_SIZE > 0) buff_count++;
    while (buff_count > 0)
    {
	    if (buff_count == 1)
        {
            read(fd, &line[0][i], (line_len) % BUFF_SIZE);
            i += (line_len) % BUFF_SIZE;
        }
	    else
        {
            read(fd, &line[0][i], BUFF_SIZE);
            i += BUFF_SIZE;
	    }
	    buff_count--;
    }
	printf("%s", line[0]);
//    if (line[0][i - 1] == '\0') return 1;
    return 0;
}

int		main(int argc, char **argv)
{
	int		state;
	int		fd;
	char	**line;

	state = 0;
	fd = open(argv[1], O_RDONLY);
	line = (char**)malloc(sizeof(char*) * 100);
	while (state < 2)
	{
	    get_next_line(fd, line);
        line++;
        state++;
    }
	close(fd);
	return (1);
}
