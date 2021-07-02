#ifndef LS_H
# define LS_H

# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <errno.h>
# include <dirent.h>
# include <sys/xattr.h>
# include <stdbool.h>
# include <pwd.h>
# include <grp.h>
# include "../libft/ft_printf/includes/header_pf.h"
/*
FLAG_l одна колонка, подробный вывод
FLAG_R рекурсия по папкам
FLAG_a скрытые файлы и папки
FLAG_r обратный порядок сортировки
FLAG_t сортировка по времени изменения, новое сверху

FLAG_u сортировка по времени последнего доступа к файлу
FLAG_f не сортировать
FLAG_g как l, но не выводит имя владельца
FLAG_d каталоги выводятся как обычные файлы, опция R игнорируется
*/

enum
{
	FLAG_l = (unsigned int)1,
	FLAG_R = (unsigned int)1 << 1,
	FLAG_a = (unsigned int)1 << 2,
	FLAG_r = (unsigned int)1 << 3,
	FLAG_t = (unsigned int)1 << 4,
	FLAG_u = (unsigned int)1 << 5,
	FLAG_f = (unsigned int)1 << 6,
	FLAG_g = (unsigned int)1 << 7,
	FLAG_d = (unsigned int)1 << 8,
};

enum
{
	LS_NOSUCHFILE,
	LS_DIR,
	LS_FILE,
};

#define SIX_MONTHS (1*60*60*24*30*6)
#define MAX(a,b) ((a) >= (b)) ? (a) : (b)
#define MAX_FILE_PATH_LEN 4096
/*
 * rights[12] - drwxrwxrwx@
 */
typedef struct s_data
{
	char			*name;
	char			*path;
	char            *link_to_file;
	double			time;
	char 			rights[12];
	u_int32_t		blocks;
	u_int32_t		links;
	char			month_day[7];
	char			time_year[6];
	char			*user;
	char			*group;
	char			major[6];
	u_int64_t		size_minor;
}					t_data;

typedef struct s_node
{
	t_data			*data;
	struct s_node	*left;
	struct s_node	*right;
	unsigned char	height;
}					t_node;

typedef struct s_dir_list
{
	char				*path;
	struct s_dir_list	*next;
}					t_dir_list;

/*
 * flag - флаги: -lRart
 * args_absent - не существующие файлы/директории
 * args_dirs - аргументы директории
 * args_files - остальные аргументы (файлы)
 * comparing_func - функция сравнения, по которой будет проходить сортировка
 * print_func - функция печати
 * head - вершина бинарного дерева
 * dir_list - список директорий, по которым надо будет пройти при рекурсии
 * dir_list_last_elem - последний элемент в списке директорий
 * num_of_nodes - число нод в дереве
 * total_for_dir
 * max_links - максимальное кол-во / разрядность ссылок
 * max_user - максимальный номер / разрядность пользователя
 * max_group - максимальный номер / разрядность группы
 * max_size - максимальный размер / разрядность файла
 * major
 * print_line
 */
typedef struct s_init
{
	unsigned int	flag;
	char            *prog_name;
	char			**args_absent;
	char			**args_dirs;
	char			**args_files;
	int				(*comparing_func)(t_data *, t_data *);
	void			(*print_func)(struct s_init *, t_node *);
	t_node			*head;
	t_dir_list		*dir_list;
	t_dir_list		*dir_list_last_elem;
	u_int64_t		num_of_nodes;
	u_int64_t		total_for_dir;
	u_int64_t		max_links;
	u_int8_t		max_user;
	u_int8_t		max_group;
	u_int64_t		max_size;
	bool			major;
	bool            print_line;
	bool            print_path;
}					t_init;

void	parse_input(int ac, char **av, t_init *init);
void	select_compare_function(t_init *init);
t_node	*balance(t_node *node);
int		sort_by_name(t_data *old_data, t_data *new_data);
int		sort_by_name_rev(t_data *old_data, t_data *new_data);
t_node	*insert_node(t_init *init, t_node *node, t_data *data);
void	myexit(t_init *init, int err);
void	free_tree(t_node *node);
void	apply_infix(t_init *init, t_node *node,
			void (*callback_func)(t_init *, t_node *));
void	select_print_function(t_init *init);
void	absent_arguments(t_init *init);
void	read_stat(t_init *init, char *path, char *name, bool show_local_dir);
void    calculate_length_for_print(t_init *init);
void    process_directories(t_init *init);
void    print_dir(t_init *init, char *path);
void	add_element_to_dir_list(t_init *init, t_node *node);
void	*free_data(t_data *data);
void	free_dir_list(t_init *init);
void	free_args(t_init *init);

#endif
