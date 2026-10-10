/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namaloma <namaloma@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:25:37 by namaloma          #+#    #+#             */
/*   Updated: 2026/10/07 18:25:40 by namaloma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_non_neg_int(const char *s, long *out)
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
	*out = n;
	return (1);
}

void	print_range_error(int i)
{
	if (i == 0)
		fprintf(stderr,
			"Error: number_of_coders must be between 1 and %d\n",
			MAX_CODERS);
	else if (i >= 1 && i <= 4)
		fprintf(stderr,
			"Error: %s must be between 1 and %d\n",
			arg_name(i), MAX_TIME);
	else if (i == 5)
		fprintf(stderr,
			"Error: number_of_compiles_required must be between 1 and %d\n",
			MAX_COMPILES);
	else if (i == 6)
		fprintf(stderr,
			"Error: dongle_cooldown must be between 0 and %d\n",
			MAX_TIME);
}

int	validate_num(char *part, int i, long *n)
{
	if (!parse_non_neg_int(part, n))
	{
		fprintf(stderr,
			"Error: %s must be a valid integer\n",
			arg_name(i));
		return (0);
	}
	if (!ok_range(i, *n))
	{
		print_range_error(i);
		return (0);
	}
	return (1);
}

int	ok_range(int idx, long n)
{
	if (idx == 0)
		return (n >= 1 && n <= MAX_CODERS);
	else if (idx >= 1 && idx <= 4)
		return (n >= 1 && n <= MAX_TIME);
	else if (idx == 5)
		return (n >= 1 && n <= MAX_COMPILES);
	else if (idx == 6)
		return (n >= 0 && n <= MAX_TIME);
	return (0);
}

char	*arg_name(int i)
{
	if (i == 0)
		return ("number_of_coders");
	if (i == 1)
		return ("time_to_burnout");
	if (i == 2)
		return ("time_to_compile");
	if (i == 3)
		return ("time_to_debug");
	if (i == 4)
		return ("time_to_refactor");
	if (i == 5)
		return ("number_of_compiles_required");
	return ("dongle_cooldown");
}
