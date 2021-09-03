#include "../includes/ls.h"

/*
 * Определение того, файл или директория пришли на вход. Если файл, то
 * возвращается значение LS_FILE, если директория, то LS_DIR, если такого
 * файла/директории не существует, то LS_NOSUCHFILE.
 */
int	dir_or_file(t_init *init, char *av)
{
	struct stat buf;

	if (stat(av, &buf) == 0 || lstat(av, &buf) == 0)
	{
		if (init->flag & FLAG_l)
			lstat(av, &buf);
		if (S_ISDIR(buf.st_mode) && !(init->flag & FLAG_d))
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
	av += ac - argnum - 1;
	while (--argnum >= 0 && ++av)
	{
		if (*av)
		{
			res = dir_or_file(input, *av);
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
void parse_flags(t_init *init, char *str)
{
	while (*str)
	{
		if (*str == 'l')
			init->flag |= FLAG_l;
		else if (*str == 'R')
			init->flag |= FLAG_R;
		else if (*str == 'a')
			init->flag |= FLAG_a;
		else if (*str == 'r')
			init->flag |= FLAG_r;
		else if (*str == 't')
			init->flag |= FLAG_t;
		else if (*str == 'u')
			init->flag |= FLAG_u;
		else if (*str == 'f')
			init->flag |= FLAG_f | FLAG_a;
		else if (*str == 'g')
			init->flag |= FLAG_g | FLAG_l;
		else if (*str == 'd')
			init->flag |= FLAG_d;
		else if (*str == 'G')
			init->flag |= FLAG_G;
		else if (*str == '1')
		{
			init->flag &= ~(FLAG_l | FLAG_g);
			init->flag |= FLAG_1;
		}
		else
		{
			fd_printf(2, "%s: illegal option -- %c\n", init->prog_name, *str);
			fd_printf(2, "usage: ft_ls [-1Radfglrtu] [file ...]\n");
			exit(1);
		}
		str++;
	}
	if (init->flag & FLAG_d)
		init->flag = (init->flag | FLAG_R) - FLAG_R;
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

	i = 0;
	res = 0;
	while (++i < ac)
	{
		if (av[i][0] == '-' && ft_strcmp(av[i], "-") != 0)
        {
		    if (ft_strcmp("--", av[i]) == 0)
            {
		        i++;
		        break;
            }
		    parse_flags(input, av[i] + 1);
        }
		else
		    break;
	}
	if (i != ac)
		res = parse_args(ac, av, ac - i, input);
	if (res != 0)
		myexit(input, res);

}
