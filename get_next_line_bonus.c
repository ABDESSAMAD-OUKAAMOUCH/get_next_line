/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoukaamo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 18:48:30 by aoukaamo          #+#    #+#             */
/*   Updated: 2025/11/19 18:48:35 by aoukaamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*get_line(char *cache)
{
	char	*line;
	size_t	total;
	size_t	i;

	if (cache[0] == '\0')
		return (NULL);
	i = 0;
	while (cache[i] && cache[i] != '\n')
		i++;
	total = i + 1 + (cache[i] == '\n');
	line = malloc(total);
	if (!line)
		return (NULL);
	ft_strlcpy(line, cache, total);
	return (line);
}

static char	*check_cache(char *cache, char *buff, ssize_t bytes_read)
{
	if (bytes_read == 0)
		return (cache);
	buff[bytes_read] = '\0';
	if (cache == NULL)
		cache = ft_strdup(buff);
	else
		cache = ft_strjoin(cache, buff);
	return (cache);
}

static char	*read_to_cache(int fd, char *cache)
{
	char	*buff;
	ssize_t	bytes_read;

	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (free(cache), NULL);
	bytes_read = 1;
	while (ft_strchr(cache, '\n') == NULL && bytes_read > 0)
	{
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buff);
			free(cache);
			return (NULL);
		}
		cache = check_cache(cache, buff, bytes_read);
		if (cache == NULL)
		{
			free(buff);
			return (NULL);
		}
	}
	free(buff);
	return (cache);
}

static char	*remove_line_cache(char *cache)
{
	char	*after_line;
	int		i;

	i = 0;
	while (cache[i] && cache[i] != '\n')
		i++;
	if (cache[i] == '\0')
	{
		free(cache);
		return (NULL);
	}
	after_line = ft_strdup(cache + i + 1);
	free(cache);
	return (after_line);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*cache[1024];

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(cache[fd]);
		cache[fd] = NULL;
		return (NULL);
	}
	cache[fd] = read_to_cache(fd, cache[fd]);
	if (cache[fd] == NULL)
		return (NULL);
	line = get_line(cache[fd]);
	if (!line)
	{
		free(cache[fd]);
		cache[fd] = NULL;
		return (NULL);
	}
	cache[fd] = remove_line_cache(cache[fd]);
	return (line);
}
