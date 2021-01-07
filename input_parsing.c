#include "ls.h"

int	dir_or_file(char *av)
{

}

/*
 * Парсинг аргументов не-флагов - пути для вывода. Принимает кол-во
 * аргументов и сами аргументы. Возвращает массив строк путей.
 */
int parse_args(int ac, char **av, int argnum, t_init *input)
{
	int	dir;
	int file;
	int res;

	if (!(input->args_dirs = (char **)ft_memalloc(sizeof(char *) * (argnum + 1))) ||
			!(input->args_files = (char **)ft_memalloc(sizeof(char *) * (argnum + 1))))
		return (ENOMEM);
	dir = -1;
	file = -1;
	while (--ac > 0 && ++av)
	{
		if (*av && *av[0] != '-')
		{
			res = dir_or_file(*av);
			if (res == LS_NOSUCHFILE)
				printf("ft_ls: %s: No such file or directory\n", *av);
			else if (res == LS_DIR)
				input->args_dirs[++dir] = *av;
			else
				input->args_files[++file] = *av;
		}
	}
	return (0);
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
 * Запись файлов и директорий по отдельности в структуру input.
 */
void split_dirs_and_files(char **args, t_init *input)
{
	int i;

	if (args == NULL)
		return;
	input->args_dirs = (char **)ft_memalloc(sizeof(char *) );
	i = 0;
	while (args[i])
	{

		i++;
	}
}

/*
 * Парсинг входных аргументов. Принимает кол-во аргументов, сами аргументы и
 * начальную структуру, в которую будет положен результат обработки. Структура
 * наполняется полями флаги (битовая маска) и аргументы (массива строк).
 */
void parse_input(int ac, char **av, t_init *input)
{
	int				i;
	int 			argnum;
	int				res;
	unsigned char   flags;

	i = 0;
	flags = 0;
	argnum = 0;
	res = 0;
	while (++i < ac)
	{
		if (av[i][0] == '-')
			flags = flags | parse_flags(av[i] + 1);
		else
			argnum++;
	}
	input->flag = flags;
	if (argnum != 0)
		res = parse_args(ac, av, argnum, input);
	if (res != 0)
		myexit(input, res);
}
