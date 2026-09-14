#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdio.h>
# include <limits.h>

typedef struct s_coder
{
	int         id;
	int         compile_count;
	long        last_compile_start;
	pthread_t   thread;
} t_coder;

typedef struct s_coders
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int     number_of_compiles_required;
	int     dongle_cooldown;
	char    *scheduler;
	t_coder *coders;
} t_coders;

#endif