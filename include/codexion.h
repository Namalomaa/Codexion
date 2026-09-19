#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>

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



// main
void init_coders(long *numeric, char *scheduler, t_coders *coders);

// parsing
int		parse_and_validate(int argc, char **argv, t_coders *coders);
void	init_coders(long *numeric, char *scheduler, t_coders *coders);


//helpers
char	**ft_split(char const *s, char c);
void	split_free(char **s);
long	ft_atol(const char *str);
int		parse_positive_int(const char *s, long *out);
char    *str_tolower(char *s);
int		len(char **s);


#endif