#ifndef LS_LS_H
#define LS_LS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

#include "libft/ft_printf/includes/header_pf.h"

enum
{
	FLAG_l = (unsigned int)1,		//одна колонка, подробный вывод
	FLAG_R = (unsigned int)1 << 1,	//рекурсия по папкам
	FLAG_a = (unsigned int)1 << 2,	//скрытые файлы и папки
	FLAG_r = (unsigned int)1 << 3,	//обратный порядок сортировки
	FLAG_t = (unsigned int)1 << 4,	//сортировка по времени изменения, новое сверху
	FLAG_u = (unsigned int)1 << 5,	//сортировка по времени последнего доступа к файлу
	FLAG_f = (unsigned int)1 << 6,	//не сортировать
	FLAG_g = (unsigned int)1 << 7,	//как l, но не выводит имя владельца
	FLAG_d = (unsigned int)1 << 8,	//каталоги выводятся как обычные файлы, опция R игнорируется
};

enum
{
	LS_NOSUCHFILE,
	LS_DIR,
	LS_FILE,
};

typedef struct		s_data
{
	char			*name;
	struct stat		*stat;
	struct dirent	*dirent;
	//////////////////////////
}					t_data;

typedef struct		s_node
{
	t_data			*data;
	struct s_node	*left;
	struct s_node	*right;
	unsigned char	height;
}					t_node;

typedef struct		s_dir_list
{
	t_data				*data;
	struct s_dir_list	*next;
}					t_dir_list;

/*
 * flag - флаги: -lRart;
 * args_dirs - аргументы директории
 * args_files - остаьные аргументы (файлы)
 * comparing_func - функция сравнения, по которой будет проходить сортировка
 * print_func - функция печати
 * head - вершина бинарного дерева
 * dir_list - не помню
 */
typedef struct      s_init
{
	unsigned int	flag;
	char 			**args_absent;
	char            **args_files;
	char            **args_dirs;
	int				(*comparing_func)(t_data *, t_data *);
	void			(*print_func)(t_node *);
	t_node			*head;
	t_dir_list		*dir_list;
}                   t_init;

void	parse_input(int ac, char **av, t_init *init);
void	select_compare_function(t_init *init);
t_node	*balance(t_node *node);
int		sort_by_name(t_data *old_data, t_data *new_data);
int		sort_by_name_rev(t_data *old_data, t_data *new_data);
t_node	*insert_node(t_init *init, t_node *node, t_data *data);
void	myexit(t_init *input, int err);
void	free_tree(t_node *node);
void	apply_infix(t_init *init, t_node *node, void (*callback_func)(t_init *, t_node *));
void	select_print_function(t_init *init);
void	absent_arguments(t_init *init);


void test_parsing(t_init *init);

#endif //LS_LS_H