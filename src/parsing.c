/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namaloma <namaloma@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 19:25:43 by namaloma          #+#    #+#             */
/*   Updated: 2026/09/22 18:07:23 by namaloma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long    *validate_split(char **parts)
{
	int		i;
	long	n;
	long    *nums;

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
			fprintf(stderr, "Error: '%s' is not a valid positive integer\n", parts[i]);
			return (free(nums), NULL);
		}
		nums[i] = n;
		i ++;
	}
	return (nums);
}

// static int total_length(int argc, char **argv)
// {
// 	int i;
// 	int len;

// 	len = 0;
// 	i = 1; // start from argv[1]
// 	while(i < argc)
// 	{
// 		len += strlen(argv[i]) + 1;
// 		i ++;
// 	}
// 	return (len);
// }


static char *get_all_args_as_strings(int argc, char **argv)
{
	char    *all;
	int     total;
	int     i;
	int     j;
	int     idx;

	total = 0;
	i = 1;
	while (i < argc)
	{
		total += strlen(argv[i]) + 1;
		i ++;
	}
	all = malloc(sizeof(char) * (total + 1));
	if (!all)
		return (NULL);
	idx = 0;
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
			all[idx ++] = argv[i][j ++];
		if (i < argc - 1)
			all[idx ++] = ' ';
		i ++;
	}
	all[idx] = '\0';
	return (all);
	//returns a string containing all args seperated by spaces.
}

int parse_and_validate(int argc, char **argv, t_coders *coders)
{
	char    *all_str; // Holds all the arguments as a single string
	char    **numeric_part;// Holds the split parts of the arguments
	long    *valid_nums; // the first 7 args as int
	char    *scheduler;

	if (argc < 2)
		return (fprintf(stderr, "Error: no arguments provided\n"), 0);
	all_str = get_all_args_as_strings(argc, argv);
	if (! all_str)
		return (fprintf(stderr, "Error: malloc failed\n"), 0);
	numeric_part = ft_split(all_str, ' ');
	if (!numeric_part)
	{
		fprintf(stderr, "Error: malloc failed\n");
		return (free(all_str), 0);
	}
	if (len(numeric_part) != 8)
	{
		fprintf(stderr, "Error: Invalid Number of arguments, Required 8\n");
		return (free(all_str), split_free(numeric_part), 0);
	}
	scheduler = numeric_part[7];
	scheduler = str_tolower(scheduler);
	if (strcmp(scheduler, "edf") != 0
		&& strcmp(scheduler, "fifo") != 0)
	{
		fprintf(stderr, "Error: scheduler must be 'fifo' or 'edf'\n");
		return (free(all_str), split_free(numeric_part), 0);
	}
	valid_nums = validate_split(numeric_part);
	if (!valid_nums)
		return(free(all_str), split_free(numeric_part), 0);
	init_coders(valid_nums, scheduler, coders);
	split_free(numeric_part);
	free (all_str);
	free (valid_nums);
	return (1);
}
