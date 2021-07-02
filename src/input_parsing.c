#include "../includes/ls.h"

/*
 * Определение того, файл или директория пришли на вход. Если файл, то
 * возвращается значение LS_FILE, если директория, то LS_DIR, если такого
 * файла/директории не существует, то LS_NOSUCHFILE.
 */
int	dir_or_file(char *av)
{
	struct stat buf;

	if (stat(av, &buf) == 0)
	{
		if (S_ISDIR(buf.st_mode))
			return (LS_DIR);
		else
			return (LS_FILE);
	}
	return (LS_NOSUCHFILE);
}

/*
 * Парсинг аргументов не-флагов - пути для вывода. Принимает кол-во
 * аргументов, сами аргументы, кол-во аргументов не-флагов и начальную
 * структуру. Записывает в структуру отдельно аргументы-директории и
 * аргументы-файлы. Возвращает 0 при успехе или ENOMEM при нехватке памяти
 * для маллока.
 */
int parse_args(int ac, char **av, int argnum, t_init *input)
{
	int	dir;
	int file;
	int res;
	int abs;

	if (!(input->args_dirs = (char **)ft_memalloc(sizeof(char *) * (argnum + 1))) ||
			!(input->args_files = (char **)ft_memalloc(sizeof(char *) * (argnum + 1))) ||
			!(input->args_absent = (char **)ft_memalloc(sizeof(char *) * (argnum + 1))))
		return (ENOMEM);
	dir = -1;
	file = -1;
	abs = -1;
	while (--ac > 0 && ++av)
	{
		if (*av && (*av[0] != '-' || ft_strcmp(*av, "-") == 0))
		{
			res = dir_or_file(*av);
			if (res == LS_NOSUCHFILE)
				input->args_absent[++abs] = *av;
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
unsigned char parse_flags(char *str, char *prog_name)
{
	unsigned char num;

	num = 0;
	while (*str)
	{
		if (*str == 'l')
			num += FLAG_l;
		else if (*str == 'R')
			num += FLAG_R;
		else if (*str == 'a')
			num += FLAG_a;
		else if (*str == 'r')
			num += FLAG_r;
		else if (*str == 't')
			num += FLAG_t;
		else if (*str == 'f')
			num += FLAG_f;
		else if (*str == '1')
		    ;
		else
		{
			printf("%s: illegal option -- %c\n", prog_name, *str);
			printf("usage: %s [-Ralrt] [file ...]\n", prog_name);
			exit(1);
		}
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
	int				res;
	unsigned char   flags;

	i = 0;
	flags = 0;
	res = 0;
	while (++i < ac)
	{
		if (av[i][0] == '-' && ft_strcmp(av[i], "-") != 0)
        {
		    if (ft_strcmp("--", av[i]) == 0)
		        continue;
		    flags = flags | parse_flags(av[i] + 1, av[0]);
        }
		else
			break;
	}
	input->flag = flags;
	if (i != ac)
		res = parse_args(ac, av, ac - i, input);
	if (res != 0)
		myexit(input, res);
}
