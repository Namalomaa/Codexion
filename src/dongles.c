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
