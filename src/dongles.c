# include "codexion.h"


void    try_grant_req(t_coders *coders)
{
    t_request   *request;

    if (coders->queue.size == 0)
        return ;
    request = &coders->queue.request[0];
    if (dongle_is_available(request->coder))
    {
        reserve_dongles(request->coder);
        pop_heap(&coders->queue, coders);
        pthread_cond_broadcast(&coders->scheduler_cond);
    }

}

int    dongle_is_available(t_coder *coder)
{
    t_dongle    *left;
    t_dongle    *right;
    long        now;

    left = coder->left_dongle;
    right = coder->right_dongle;
    now = get_current_time();
    return (
        left->reserved == 0
        && right->reserved == 0
        && now >= left->cold_at
        && now >= right->cold_at
    );
}

void    reserve_dongles(t_coder *coder)
{
    coder->right_dongle->reserved = 1;
    coder->left_dongle->reserved = 1;
    coder->granted = 1;
}

static void    release_dongles(t_coder *coder)
{
    long    now;

    now = get_current_time();
    coder->right_dongle->reserved = 0;
    coder->right_dongle->cold_at = now + coder->simulation->dongle_cooldown;
    coder->left_dongle->reserved = 0;
    coder->left_dongle->cold_at = now + coder->simulation->dongle_cooldown;
    coder->granted = 0;
}

void    release_resources(t_coder *coder)
{
    t_coders    *coders;

    coders = coder->simulation;
    pthread_mutex_lock(&coders->scheduler_lock);
    release_dongles(coder);
    try_grant_req(coders);
    pthread_cond_broadcast(&coders->scheduler_cond);
    pthread_mutex_unlock(&coders->scheduler_lock);
}