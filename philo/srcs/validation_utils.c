/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 14:51:29 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/27 22:11:34 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	ft_isspace(int c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

static int	ft_issign(int c)
{
	return (c == 43 || c == 45);
}

static int	ft_isdigit(int c)
{
	return (c >= 48 && c <= 57);
}

size_t	sig_digits_strlen(const char *nbr)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (ft_isspace(nbr[i]))
		++i;
	if (ft_issign(nbr[i]))
		++i;
	while (nbr[i] == '0' && ft_isdigit(nbr[i + 1]))
		++i;
	j = 0;
	while (nbr[i])
	{
		if (!ft_isdigit(nbr[i]))
			return (0);
		++j;
		++i;
	}
	return (j);
}

int64_t	ft_atol(const char *nptr)
{
	size_t	i;
	int32_t	sign;
	int64_t	res;

	res = 0;
	sign = 1;
	i = 0;
	while (ft_isspace(nptr[i]))
		i++;
	if (ft_issign(nptr[i]))
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (ft_isdigit(nptr[i]))
	{
		res = res * 10 + sign * (nptr[i] - 48);
		i++;
	}
	return (res);
}
