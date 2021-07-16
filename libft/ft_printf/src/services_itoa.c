/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   services_itoa.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: couida <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:39:24 by couida            #+#    #+#             */
/*   Updated: 2020/01/17 16:39:27 by couida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/header_pf.h"

int			numlength(long long l, int base)
{
	int			i;

	i = 0;
	if (l == 0)
		return (1);
	if (base == 10 && l < 0)
	{
		i = 1;
		l = l / 10;
		l = -l;
	}
	while (l > 0)
	{
		i++;
		l /= base;
	}
	return (i);
}

t_list_pf	*new_list(int fd)
{
	t_list_pf	*list;

	list = (t_list_pf *)ft_memalloc(sizeof(t_list_pf));
	list->fd = fd;
	return (list);
}

char		*ft_itoa_base_u2(unsigned long long value,
		int base, int up, int i)
{
	char		*str1;
	char		*str2;
	char		*num;

	str1 = "0123456789ABCDEF";
	str2 = "0123456789abcdef";
	num = (char *)malloc(sizeof(char) * (i + 1));
	num[i] = '\0';
	while (--i >= 0)
	{
		if (up == 1)
			num[i] = str1[value % base];
		else
			num[i] = str2[value % base];
		value = value / base;
	}
	return (num);
}

char		*ft_itoa_base_u1(unsigned long long value,
		int base, int uppercase)
{
	unsigned long long	temp;
	int					i;

	i = 0;
	temp = value;
	if (value == 0)
		i = 1;
	while (temp > 0)
	{
		i++;
		temp /= base;
	}
	return (ft_itoa_base_u2(value, base, uppercase, i));
}

char		*ft_itoa_pf(long long i, t_list_pf *list)
{
	char	*c;
	char	*num;
	int		a;

	a = numlength(i, 10);
	c = "9223372036854775808";
	num = (char *)malloc(sizeof(char) * (a + 1));
	num[a] = '\0';
	if (i < 0)
	{
		list->flag0[2] = '-';
		if (i == -9223372036854775807 - 1)
		{
			while (--a >= 0)
				num[a] = c[a];
			return (num);
		}
		i = -i;
	}
	while (--a >= 0)
	{
		num[a] = i % 10 + '0';
		i = i / 10;
	}
	return (num);
}
