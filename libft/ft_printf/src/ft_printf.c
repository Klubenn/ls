/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: couida <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:37:51 by couida            #+#    #+#             */
/*   Updated: 2020/01/17 16:37:54 by couida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/header_pf.h"

void	separation(const char *format, va_list ap, int fd)
{
	int		i;
	int		num;
	char	*ch;


	num = 0;
	ch = (char *)format;
	while (*ch != '\0')
	{
		while (*ch && *ch != '%')
		{
			if (*ch == '{' && (i = print_color(ch, fd)))
			{
				ch += i;
				continue ;
			}
			ft_putchar_pf(*ch, 1, fd);
			ch++;
			num++;
		}
		if (*ch == '%')
		{
			ch++;
			ch += parsing1(ch, ap, fd);
		}
	}
}

int		ft_printf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	separation(format, ap, 1);
	va_end(ap);
	return (ft_putchar_pf('@', 0, 1));
}

int		fd_printf(int fd, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	separation(format, ap, fd);
	va_end(ap);
	return (ft_putchar_pf('@', 0, fd));
}
