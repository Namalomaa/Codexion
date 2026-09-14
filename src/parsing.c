#include "codexion.h"

static long    *validate_split(char **parts)
{
	int		i;
	int		j;
	long	n;
	long    *nums;

	if (!parts || !parts[0])
		return (NULL);
	i = 0;
	nums = malloc(sizeof(long) * len(parts));
	if (!nums)
		return (NULL);
	while (parts[i])
	{
		j = 0;
		if (parts[i][j] == '-' )
		{
			fprintf(stderr, "Invalid values should be positive.\n");	
			return (free(nums), NULL); // reject negative numbers
		}
		if (parts[i][j] == '+')
			j++;
		if (!parts[i][j])
			return (free(nums), NULL);
		while (parts[i][j])
		{
			if (parts[i][j] < '0' || parts[i][j] > '9')
				return (free(nums), NULL);
			j++;
		}
		n = ft_atol(parts[i]);
		nums [i] = n;
		if (n <= 0 || n > INT_MAX)
		{
			fprintf(stderr, "Invalid values should be greater than 0.\n");
			return (free(nums), NULL);
		}
		i++;
	}
	return (nums);
}

static int total_length(int argc, char **argv)
{
	int i;
	int len;

	len = 0;
	i = 1; // start from argv[1]
	while(i < argc)
	{
		len += strlen(argv[i]) + 1;
		i ++;
	}
	return (len);
}


static char *get_all_numbers_as_strings(int argc, char **argv)
{
	char    *all_nums;
	int     i;
	int     all_idx;
	int     len;
	int     start;

	len  = total_length(argc, argv);
	all_nums = malloc(sizeof(char) * (len + 1));
	if (!all_nums)
		return (NULL);
	all_idx = 0;
	start = 1;
	while (start < argc)
	{
		i = 0;
		while (argv[start][i])
			all_nums[all_idx ++] = argv[start][i ++];
		 all_nums[all_idx ++] = ' ';
		 start ++;
	}
	all_nums[all_idx - 1] = '\0';
	return (all_nums);
	//returns a string containing all nums seperated by spaces.
}

long    *parse_and_validate(int argc, char **argv, t_coders *coders)
{
	char    *all_str; // Holds all the arguments as a single string
	char    **numeric_part;// Holds the split parts of the arguments
	long    *valid_nums; // the first 7 args as int
	char    *scheduler;

	all_str = get_all_numbers_as_strings(argc, argv);
	if (! all_str)
		return (free(all_str), NULL);
	numeric_part = ft_split(all_str, ' ');
	if (!numeric_part)
		return (split_free(numeric_part), NULL);
	if (len(numeric_part) != 8)
	{
		fprintf(stderr, "Invalid Number of arguments, Required 8\n");
		return (free(all_str), split_free(numeric_part), NULL);
	}
	scheduler = numeric_part[7];
	scheduler = str_tolower(scheduler);
	numeric_part[7] = NULL;
	valid_nums = validate_split(numeric_part);
	if (!valid_nums)
		return(free(all_str), split_free(numeric_part), NULL);
	if (strcmp(scheduler, "edf") != 0
		&& strcmp(scheduler, "fifo") != 0)
	{
		fprintf(stderr, "scheduler must be 'fifo' or 'edf'\n");
		return(free(all_str), split_free(numeric_part), NULL);
	}
	coders->coders = malloc(sizeof(t_coder) * valid_nums[0]);
	if (!coders->coders)
		return (free(all_str), split_free(numeric_part), NULL);
	init_coders(valid_nums, scheduler, coders);
	split_free(numeric_part);
	free (all_str);
	return (valid_nums);
}
