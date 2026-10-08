/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parsing.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: namaloma <namaloma@learner.42.tech>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/14 19:25:43 by namaloma         #+#    #+#              */
/*   Updated: 2026/10/07 17:29:56 by namaloma        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	*validate_split(char **parts)
{
	int		i;
	long	n;
	long	*nums;

	if (!parts || !parts[0])
		return (NULL);
	nums = malloc(sizeof(long) * 7);
	if (!nums)
	{
		fprintf(stderr, "Error: malloc failed\n");
		return (NULL);
	}
	i = 0;
	while (i < 7)
	{
		if (!parse_positive_int(parts[i], &n))
		{
			fprintf(stderr,
				"Error: '%s' is not a valid positive integer\n", parts[i]);
			return (free(nums), NULL);
		}
		nums[i] = n;
		i++;
	}
	return (nums);
}

static char	*get_all_args_as_strings(int argc, char **argv)
{
	char	*all;
	int		total;
	int		i;
	int		idx;

	total = 0;
	i = 1;
	while (i < argc)
		total += strlen(argv[i++]) + 1;
	all = malloc(total + 1);
	if (!all)
		return (NULL);
	i = 1;
	idx = 0;
	while (i < argc)
	{
		idx = copy_arg(all, argv[i], idx);
		if (i++ < argc - 1)
			all[idx++] = ' ';
	}
	all[idx] = '\0';
	return (all);
}

static int	free_parse(char *all_str, char **parts, long *nums)
{
	if (nums)
		free(nums);
	if (parts)
		split_free(parts);
	if (all_str)
		free(all_str);
	return (0);
}

int	parse_and_validate(int argc, char **argv, t_coders *coders)
{
	char	*all_str;
	char	**parts;
	long	*nums;

	if (argc < 2)
		return (fprintf(stderr, "Error: no arguments provided\n"), 0);
	all_str = get_all_args_as_strings(argc, argv);
	if (!all_str)
		return (fprintf(stderr, "Error: malloc failed\n"), 0);
	parts = ft_split(all_str, ' ');
	if (!parts)
		return (free_parse(all_str, NULL, NULL));
	if (len(parts) != 8)
		return (fprintf(stderr, "Error: Invalid Number of arguments\n"),
			free_parse(all_str, parts, NULL));
	if (strcmp(parts[7], "edf") && strcmp(parts[7], "fifo"))
		return (fprintf(stderr, "Error: invalid scheduler\n"),
			free_parse(all_str, parts, NULL));
	nums = validate_split(parts);
	if (!nums)
		return (free_parse(all_str, parts, NULL));
	init_coders(nums, parts[7], coders);
	free_parse(all_str, parts, nums);
	return (1);
}
