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
#include <stdio.h>

void	copy_str(size_t len, size_t start, char *str, char const *s)
{
	size_t	i;

	i = 0;
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
}

char	*ft_substr(char const *s, size_t start, size_t len)
{
	char	*str;
	size_t	ls;

	if (s == NULL)
		return (NULL);
	ls = ft_strlen(s);
	if (start >= ls)
	{
		str = malloc(1);
		if (str == NULL)
			return (NULL);
		str[0] = '\0';
		return (str);
	}
	if (len > ls - start)
		len = ls - start;
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);
	copy_str(len, start, str, s);
	return (str);
}

char	*get_line(char **cache)
{
	char	*line;
	size_t	i;
	size_t	is_bs;
	char	*tmp;

	if (cache == NULL || *cache == NULL)
		return (NULL);
	i = 0;
	while ((*cache)[i] && (*cache)[i] != '\n')
		i++;
	is_bs = (*cache)[i] == '\n';
	line = ft_substr(*cache, 0, i + is_bs);
	tmp = ft_substr(*cache, i + is_bs, ft_strlen(*cache) - i - is_bs);
	free(*cache);
	if (tmp != NULL && tmp[0] == '\0')
	{
		free(tmp);
		tmp = NULL;
	}
	*cache = tmp;
	return (line);
}

char	*check_bytes(char **cache, int bytes_read)
{
	char	*line;

	if (bytes_read < 0)
	{
		if (*cache != NULL)
			free(*cache);
		*cache = NULL;
		return (NULL);
	}
	if (bytes_read == 0)
	{
		if (*cache == NULL)
			return (NULL);
		line = *cache;
		*cache = NULL;
		return (line);
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	char		*buff;
	static char	*cache;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (cache == NULL || ft_strchr(cache, '\n') == NULL)
	{
		buff = malloc(BUFFER_SIZE + 1);
		if (!buff)
			return (NULL);
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			free(buff);
			return (check_bytes(&cache, bytes_read));
		}
		buff[bytes_read] = '\0';
		cache = ft_strjoin(cache, buff);
		free(buff);
		if (cache == NULL)
			return (NULL);
	}
	return (get_line(&cache));
}
