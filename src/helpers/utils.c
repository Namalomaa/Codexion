/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namaloma <namaloma@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:25:17 by namaloma          #+#    #+#             */
/*   Updated: 2026/10/07 18:25:19 by namaloma         ###   ########.fr       */
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

int	print_state(t_coders *coders, int id, char *msg)
{
	int	done;
	int	count;

	if (strcmp(msg, "is compiling") == 0)
		count = get_compile_count(&coders->coders[id - 1]);
	pthread_mutex_lock(&coders->simulation_lock);
	done = 0;
	if (!coders->stop)
	{
		printf("%ld %i %s",
			get_current_time() - coders->start_time, id, msg);
		if (strcmp(msg, "is compiling") == 0)
			printf(" [%d/%d]",
				count + 1,
				coders->number_of_compiles_required);
		printf("\n");
		done = 1;
	}
	pthread_mutex_unlock(&coders->simulation_lock);
	return (done);
}

int	copy_arg(char *all, char *arg, int idx)
{
	int	j;

	j = 0;
	while (arg[j])
		all[idx++] = arg[j++];
	return (idx);
}
