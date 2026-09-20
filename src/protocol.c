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

    start = get_current_time();
    coder->last_compile_start = start;
    printf("%ld %i is compiling", start, coder->id);
    while(get_current_time() - start < coder->simulation->time_to_compile)
        usleep(1000);
    coder->compile_count ++;
}

void    *coder_protocol(void *arg)
{
    t_coder *coder;
    
    coder = arg;
    while (! coder->simulation->stop)
    {
        pickup_dongles(coder);
        if (coder->simulation->stop)
            break;
        compile(coder);
        putdown_dongles(coder);
        debug(coder);
        refactor(coder);
    }
    return (NULL);
}
