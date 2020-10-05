#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct      s_init {
	unsigned char   lRart;
	char            **args;
}                   t_init;

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

char **parse_args(int ac, char **av)
{
	char    **args;
	int     len;
	int     i;
	int     j;

	len = parse_args_num(ac, av);
	if (!(args = (char **)malloc(sizeof(char *) * (len + 1))))//todo replace to ft_memalloc
		return (NULL);
	i = 0;
	j = 0;
	while (++i < ac)
	{
		if (av[i][0] != '-')
		{
			if (!(args[j] = strdup(av[i])))//todo replace strdup
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

unsigned char parse_flags(char *str)
{
	unsigned char num;

	num = 0;
	while (*str)
	{
		if (*str == 'l')
			num = num | 1 << 4;
		else if (*str == 'R')
			num = num | 1 << 3;
		else if (*str == 'a')
			num = num | 1 << 2;
		else if (*str == 'r')
			num = num | 1 << 1;
		else if (*str == 't')
			num = num | 1;
		else
			printf("Unknown flag -%c\n", *str);//todo replace with ft_printf
		str++;
	}
	return (num);
}

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
	input->lRart = flags;
}

int main(int argc, char **argv) {
	t_init input;

	parse_input(argc, argv, &input);
	return 0;
}
