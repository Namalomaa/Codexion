# include "codexion.h"

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

void    release_dongles(t_coder *coder)
{
    long    now;

    now = get_current_time();
    coder->right_dongle->reserved = 0;
    coder->right_dongle->cold_at = now + coder->simulation->dongle_cooldown;
    coder->left_dongle->reserved = 0;
    coder->right_dongle->cold_at = now + coder->simulation->dongle_cooldown;
    coder->granted = 0;
}