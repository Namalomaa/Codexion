/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   helpers.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: namaloma <namaloma@learner.42.tech>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/14 19:25:25 by namaloma         #+#    #+#              */
/*   Updated: 2026/10/07 17:31:52 by namaloma        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	len(char **s)
{
	int	count;

	count = 0;
	while (s[count])
		count++;
	return (count);
}

void	split_free(char **s)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

char	*ft_strdup(const char *s)
{
	char	*ds;
	size_t	i;
	size_t	len;

	len = 0;
	while (s[len])
	{
		len++;
	}
	ds = (char *) malloc(sizeof(char) * (len + 1));
	if (!ds)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ds[i] = s[i];
		i++;
	}
	ds[i] = '\0';
	return (ds);
}

int	parse_positive_int(const char *s, long *out)
{
	long	n;
	int		i;
	int		digit;

	if (!s || !s[0])
		return (0);
	i = 0;
	if (s[i] == '+')
		i++;
	if (!s[i])
		return (0);
	n = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		digit = s[i] - '0';
		if (n > (INT_MAX - digit) / 10)
			return (0);
		n = n * 10 + digit;
		i++;
	}
	if (n < 0)
		return (0);
	*out = n;
	return (1);
}
