/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atoulous <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 20:36:52 by atoulous          #+#    #+#             */
/*   Updated: 2016/05/20 14:51:00 by atoulous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_line	*get_fd(int fd, t_line **begin)
{
	t_line	*elem;

	elem = *begin;
	if (!elem->fd)
	{
		elem->fd = fd;
		return (elem);
	}
	while (elem->fd != fd && elem)
	{
		if (!elem->next)
		{
			elem->next = (t_line*)ft_memalloc(sizeof(t_line));
			elem->next->fd = fd;
			return (elem->next);
		}
		elem = elem->next;
	}
	return (elem);
}

static int		put_next_line(char **line, char *buf, t_line *elem, int ret)
{
	size_t	i;

	TMP2 = ft_strjoin(TMP, buf);
	free(TMP);
	TMP = ft_strdup(TMP2);
	free(TMP2);
	i = 0;
	while (TMP[i] != '\n' && TMP[i])
		i++;
	if (TMP[i] == '\n')
	{
		*line = ft_strndup(TMP, i);
		TMP2 = ft_strdup(TMP);
		free(TMP);
		TMP = ft_strsub(TMP2, i + 1, ft_strlen(TMP2));
		free(TMP2);
		return (ret < BUFF_SIZE && !TMP ? 0 : 1);
	}
	if (TMP[i] == '\0' && ret < BUFF_SIZE)
	{
		*line = ft_strndup(TMP, i);
		ft_bzero(TMP, ft_strlen(TMP));
		return (i ? 1 : 0);
	}
	return (0);
}

int				get_next_line(const int fd, char **line)
{
	static t_line	*begin = NULL;
	t_line			*elem;
	char			buf[BUFF_SIZE + 1];
	int				ret;

	if (fd < 0 || !line || read(fd, *line, 0) < 0)
		return (-1);
	if (!begin)
		begin = (t_line*)ft_memalloc(sizeof(t_line));
	ft_bzero(buf, BUFF_SIZE + 1);
	if (fd >= 0 && (elem = get_fd(fd, &begin)))
	{
		if (!TMP)
			TMP = ft_strnew(BUFF_SIZE);
		while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
		{
			if (put_next_line(line, buf, elem, ret))
				return (1);
			ft_bzero(buf, ft_strlen(buf));
		}
		if (TMP && ret == 0)
			if (put_next_line(line, buf, elem, ret))
				return (1);
	}
	return (0);
}
