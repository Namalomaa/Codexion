#include "../include/codexion.h"

static void init_coders(char **argv, t_coders *coders)
{
	int i;

	i = 0;
	while (i < coders->number_of_coders)
	{
		coders->coders[i].id = i + 1;
		coders->coders[i].compile_count = 0;
		coders->coders[i].last_compile_start = 0;
		i++;
	}

	coders->number_of_coders = ft_atol(argv[1]);
    coders->time_to_burnout = ft_atol(argv[2]);
    coders->time_to_compile = ft_atol(argv[3]);
    coders->time_to_debug = ft_atol(argv[4]);
    coders->time_to_refactor = ft_atol(argv[5]);
    coders->number_of_compiles_required = ft_atol(argv[6]);
    coders->dongle_cooldown = ft_atol(argv[7]);
    coders->scheduler = argv[8];

}

int	main(int argc, char **argv)
{
	t_coders	coders;
	if (argc != 9)
	{
		fprintf(stderr,
			"Error: Invalid number of arguments '%d' required '%d'.\n",
			argc - 1, 8);
		fprintf(stderr, "Usage: %s <number_of_coders>", argv[0]);
		fprintf(stderr, " <time_to_burnout> <time_to_compile> <time_to_debug>");
		fprintf(stderr, " <time_to_refactor> <number_of_compiles_required>");
		fprintf(stderr, " <dongle_cooldown> <scheduler>\n");
		return (1);
	}
	init_coders(argv, &coders);
	parse_and_validate(argc, argv, &coders);
}
