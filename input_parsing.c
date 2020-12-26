#include "ls.h"

/*
 * Подсчёт количества аргументов не-флагов. Возвращает число.
 */
int parse_args_num(int ac, char **av)
{
	int i;
	int num;

	i = 0;
	num = 0;
	while (++i < ac)
	{
		if (av[i][0] != '-')
			num++;
	}
	return num;
}

/*
 * Парсинг аргументов не-флагов - пути для вывода. Принимает кол-во
 * аргументов и сами аргументы. Возвращает массив строк путей.
 */
char **parse_args(int ac, char **av)
{
	char    **args;
	int     len;
	int     i;
	int     j;

	len = parse_args_num(ac, av);
	if (len == 0 || !(args = (char **)ft_memalloc(sizeof(char *) * (len + 1))))
		return (NULL);
	i = 0;
	j = 0;
	while (++i < ac)
	{
		if (av[i][0] != '-')
		{
			if (!(args[j] = ft_strdup(av[i])))
			{
				while (--j >= 0)
					free(args[j]);
				free(args);
				return (NULL);
			}
			j++;
		}
	}
	return (args);
}

/*
 * Парсинг флагов. На вход принимается строка. Возвращается битовая маска
 * размера 1 байт.
 */
unsigned char parse_flags(char *str)
{
	unsigned char num;

	num = 0;
	while (*str)
	{
		if (*str == 'l')
			num = flag_l;
		else if (*str == 'R')
			num = flag_R;
		else if (*str == 'a')
			num = flag_a;
		else if (*str == 'r')
			num = flag_r;
		else if (*str == 't')
			num = flag_t;
		else
			printf("Unknown flag -%c\n", *str);//todo replace with ft_printf
		str++;
	}
	return (num);
}

/*
 * Парсинг входных аргументов. Принимает кол-во аргументов, сами аргументы и
 * начальную структуру, в которую будет положен результат обработки. Структура
 * наполняется полями флаги (битовая маска) и аргументы (массива строк).
 */
void parse_input(int ac, char **av, t_init *input)
{
	int				i;
	unsigned char   flags;

	i = 0;
	flags = 0;
	while (++i < ac)
	{
		if (av[i][0] == '-')
			flags = flags | parse_flags(av[i] + 1);
	}
	input->args = parse_args(ac, av);
	input->flag = flags;
}
