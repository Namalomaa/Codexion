#include "codexion.h"

void init_coders(long *numeric, char *scheduler, t_coders *coders)
{
	int i;

	i = 0;
	coders->number_of_coders = numeric[0];
	while (i < coders->number_of_coders)
	{
		coders->coders[i].id = i + 1;
		coders->coders[i].compile_count = 0;
		coders->coders[i].last_compile_start = 0;
		i++;
	}

    coders->time_to_burnout = numeric[1];
    coders->time_to_compile = numeric[2];
    coders->time_to_debug = numeric[3];
    coders->time_to_refactor = numeric[4];
    coders->number_of_compiles_required = numeric[5];
    coders->dongle_cooldown = numeric[6];
    coders->scheduler = scheduler;

}

int	main(int argc, char **argv)
{
	t_coders	coders;

    if (!parse_and_validate(argc, argv, &coders))
		return (1);
	
}
