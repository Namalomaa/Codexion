#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <time.h>
# include <unistd.h>

typedef struct s_coders t_coders;

typedef struct  s_dongle
{
    pthread_mutex_t dongle;
	int				reserved;
    long            cold_at;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				compile_count;
	long			last_compile_start;
	int				granted;
	pthread_t		thread;
	t_dongle		*left_dongle;
    t_dongle		*right_dongle;
    t_coders    	*simulation;
}	t_coder;

typedef struct  s_request
{
    t_coder *coder;
    long    request_time;
    long    deadline;

}   t_request;

typedef struct  s_queue
{
    t_request   *request;
    int         size;
    int         capacity;
}   t_queue;

typedef struct s_coders
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	long			start_time;
    int				stop;
	char			*scheduler;
	pthread_t		monitor_thread;
	t_dongle		*dongles;
    // long            *dongle_cold_at;
    pthread_mutex_t	simulation_lock;
	pthread_cond_t  simulation_cond;
	pthread_mutex_t	scheduler_lock;
	pthread_cond_t	scheduler_cond;
	t_coder 		*coders;
    t_queue         queue;
}	t_coders;



// initilizing
void	init_coders(long *numeric, char *scheduler, t_coders *coders);
int		init_dongles(t_coders *coders);

// parsing
int		parse_and_validate(int argc, char **argv, t_coders *coders);
void	init_coders(long *numeric, char *scheduler, t_coders *coders);


// helpers
char	**ft_split(char const *s, char c);
void	split_free(char **s);
long	ft_atol(const char *str);
char	*ft_strdup(const char *s);
int		parse_positive_int(const char *s, long *out);
char    *str_tolower(char *s);
int		len(char **s);


// getters and setters
long    get_current_time();
long    get_last_compile_start(t_coder *coder);
int     get_compile_count(t_coder *coder);
int     get_stop(t_coders *coders);
void    set_stop(t_coders *coders, int value);
void    set_last_compile_start(t_coder *coder, long value);
void    set_compile_count(t_coder *coder, int value);


void    *coder_protocol(void *coder);
void    compile(t_coder *coder);
void    debug(t_coder *coder);
void    refactor(t_coder *coder);
int		pickup_dongles(t_coder *coder);
void    putdown_dongles(t_coder *coder);
int 	print_state(t_coders *coders, int id, char *msg);

void    *coders_monitor(void *arg);
int	 	print_burnout(t_coders *coders, int id);

#endif
