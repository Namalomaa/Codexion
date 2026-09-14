# include "../include/codexion.h"

int	validate_split(char **parts)
{
	int		i;
	int		j;
	long	n;

	if (!parts || !parts[0])
		return (0);
	i = 0;
	while (parts[i])
	{
		j = 0;
        if (parts[i][j] == '-' )
            return (0); // reject negative numbers
		if (parts[i][j] == '+')
			j++;
		if (!parts[i][j])
			return (0);
		while (parts[i][j])
		{
			if (parts[i][j] < '0' || parts[i][j] > '9')
				return (0);
			j++;
		}
		n = ft_atol(parts[i]);
		if (n < INT_MIN || n > INT_MAX)
			return (0);
		i++;
	}
	return (1);
}

static  int total_length(int argc, char **argv)
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


char	*get_all_numbers_as_strings(int argc, char **argv)
{
    chat    *all_nums;
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
         start_idx ++;
    }
    all_nums[all_idx - 1] = '\0';
    return (all_nums);
    //returns a string containing all nums seperated by spaces.
}

int *parse_and_validate(int argc, char **argv, t_coders *coders)
{
    char    *all_str; // Holds all the arguments as a single string
    char    *parts[8];// Holds the split parts of the arguments
    int     nums[8]; // Holds the converted integer values of the arguments

    all_str =  get_all_numbers_as_strings(argc, argv);
    if (! all_str)
        return (NULL);
    parts = ft_split(all_str, ' ');
    free (all_str);
    if (!parts || !validate_split(parts))
        return (split_free(parts), NULL);
    return (nums)
}