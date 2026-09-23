#include "codexion.h"

long    get_current_time()
{
    struct  timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (time.tv_sec * 1000 + time.tv_nsec / 1000000);
}

void    compile(t_coder *coder)
{
    long    start;
    long    elapsed_time;

    start = get_current_time();
    set_last_compile_start(coder, start);
    elapsed_time = start - coder->simulation->start_time;
    if (!get_stop(coder->simulation))
        printf("%ld %i is compiling\n", elapsed_time, coder->id);
    while(!get_stop(coder->simulation) &&
           get_current_time() - start < coder->simulation->time_to_compile)
        usleep(1000);
    if (!get_stop(coder->simulation))
        set_compile_count(coder, get_compile_count(coder) + 1);
}

void    debug(t_coder *coder)
{
    long    start;
    long    elapsed_time;

    start = get_current_time();
    elapsed_time = start - coder->simulation->start_time;
    if (!get_stop(coder->simulation))
        printf("%ld %i is debugging\n", elapsed_time, coder->id);        
    while(!get_stop(coder->simulation) &&
           get_current_time() - start < coder->simulation->time_to_debug)
        usleep(1000);
}

void    refactor(t_coder *coder)
{
    long    start;
    long    elapsed_time;

    start = get_current_time();
    elapsed_time = start - coder->simulation->start_time;
    if (!get_stop(coder->simulation))
        printf("%ld %i is refactoring\n", elapsed_time, coder->id);
    while(!get_stop(coder->simulation) &&
           get_current_time() - start < coder->simulation->time_to_refactor)
        usleep(1000);
}

void    pickup_dongles(t_coder *coder)
{
    if (coder->id == coder->simulation->number_of_coders)
    {
        pthread_mutex_lock(coder->right_dongle);
        if (!get_stop(coder->simulation))
        {
            printf("%ld %i has taken a dongle\n",
            get_current_time() - coder->simulation->start_time,
            coder->id);
        }
        pthread_mutex_lock(coder->left_dongle);
        if (!get_stop(coder->simulation))
        {
            printf("%ld %i has taken a dongle\n",
            get_current_time() - coder->simulation->start_time,
            coder->id);
        }
    }
    else
    {
        pthread_mutex_lock(coder->left_dongle);
        if (!get_stop(coder->simulation))
        {
            printf("%ld %i has taken a dongle\n",
            get_current_time() - coder->simulation->start_time,
            coder->id);
        }
        pthread_mutex_lock(coder->right_dongle);
        if (!get_stop(coder->simulation))
        {
            printf("%ld %i has taken a dongle\n",
            get_current_time() - coder->simulation->start_time,
            coder->id);
        }
    }
}

void    putdown_dongles(t_coder *coder)
{
    pthread_mutex_unlock(coder->left_dongle);
    pthread_mutex_unlock(coder->right_dongle);
}


void    *coder_protocol(void *arg)
{
    t_coder *coder;
    
    coder = arg;
    while (!get_stop(coder->simulation))
    {
        pickup_dongles(coder);
        if (get_stop(coder->simulation))
        {
            putdown_dongles(coder);
            break;
        }
        compile(coder);
        putdown_dongles(coder);
        if (get_stop(coder->simulation))
            break;
        debug(coder);
        if (get_stop(coder->simulation))
            break;
        refactor(coder);
    }
    return (NULL);
}
