/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoukaamo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 20:14:48 by aoukaamo          #+#    #+#             */
/*   Updated: 2025/11/14 20:26:06 by aoukaamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
		return (NULL);
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

static char	*after_the_line(char *cache)
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
	static char	*cache;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(cache);
		cache = NULL;
		return (NULL);
	}
	cache = read_to_cache(fd, cache);
	if (cache == NULL)
		return (NULL);
	line = get_line(cache);
	if (!line)
	{
		free(cache);
		cache = NULL;
		return (NULL);
	}
	cache = after_the_line(cache);
	return (line);
}
