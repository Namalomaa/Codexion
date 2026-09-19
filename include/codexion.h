#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>

typedef struct s_coder
{
	int				id;
	int				compile_count;
	long			last_compile_start;
	pthread_t		thread;
	pthread_mutex_t	*left_dongle;
	pthread_mutex_t	*right_dongle;
}	t_coder;

typedef struct s_coders
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
	pthread_mutex_t	*dongles;
	t_coder *coders;
}	t_coders;



// initilizing
void	init_coders(long *numeric, char *scheduler, t_coders *coders);
void	init_dongles(t_coders *coders);

// parsing
int		parse_and_validate(int argc, char **argv, t_coders *coders);
void	init_coders(long *numeric, char *scheduler, t_coders *coders);


//helpers
char	**ft_split(char const *s, char c);
void	split_free(char **s);
long	ft_atol(const char *str);
char	*ft_strdup(const char *s);
int		parse_positive_int(const char *s, long *out);
char    *str_tolower(char *s);
int		len(char **s);


void    *coder_routine(void *coder);
#endif
