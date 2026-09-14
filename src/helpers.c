#include "codexion.h"

int len(char **s)
{
    int count;

    count = 0;
    while (s[count])
        count ++;
    return (count);
}

void	split_free(char **s)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		free (s[i]);
		i ++;
	}
	free (s);
}

char    *str_tolower(char *s)
{
    int i;

    i = 0;
    if(!s)
        return(s);
    while(s[i])
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] += 32;
        i ++;
    }
	return (s);
}

long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	size_t	i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
    return (result * sign);
}
