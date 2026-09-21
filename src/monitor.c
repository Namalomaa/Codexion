# include "codexion.h"

void    *coders_monitor(void *arg)
{
    t_coders *coders;
    int       i;
    int       all_compiled;

    coders = arg;
    while (!get_stop(coders))
    {
        all_compiled = 0;
        i = 0;
        while (i < coders->number_of_coders)
        {
            if (get_current_time() - get_last_compile_start(&coders->coders[i]) >= coders->time_to_burnout)
            {
                printf("%ld %i has burned out\n",
                    get_current_time() - coders->start_time,
                    coders->coders[i].id);
                set_stop(coders, 1);
                break ;
            }
            if (get_compile_count(&coders->coders[i]) >= coders->number_of_compiles_required)
                all_compiled ++;
            i ++;
        }
        if (all_compiled >= coders->number_of_coders)
        {
            set_stop(coders, 1);
            break ;
        }
        usleep(1000);
    }
    return (NULL);
}