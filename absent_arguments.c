#include "ls.h"

static int	length(char **str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

static void	sort_absent_arguments(char **args, int len)
{
	char *tmp;
	int sort_flag;
	int i;

	while(1)
	{
		sort_flag = 0;
		i = 0;
		while(i < len - 1)
		{
			if (ft_strcmp(args[i], args[i + 1]) > 0)
			{
				tmp = args[i];
				args[i] = args[i + 1];
				args[i + 1] = tmp;
				sort_flag = 1;
			}
			i++;
		}
		if (sort_flag == 0)
			break;
	}
}

static void print_absent_arguments(char **args)
{
	int i;

	i = 0;
	while(args[i])
	{
		printf("ft_ls: %s: No such file or directory\n", args[i]);
		i++;
	}
}

void	absent_arguments(t_init *init)
{
	if ((init->flag & FLAG_f) == 0)
		sort_absent_arguments(init->args_absent, length(init->args_absent));
	print_absent_arguments(init->args_absent);
}
