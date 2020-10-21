#ifndef LS_LS_H
#define LS_LS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "libft.c" //todo remove

enum
{
	flag_l = 1,		//одна колонка, подробный вывод
	flag_R = (unsigned int)1 << 1,	//рекурсия по папкам
	flag_a = (unsigned int)1 << 2,	//скрытые файлы и папки
	flag_r = (unsigned int)1 << 3,	//обратный порядок сортировки
	flag_t = (unsigned int)1 << 4,	//сортировка по времени изменения, новое сверху
};

typedef struct	s_node t_node;

typedef struct	s_data
{
	char	*name;
	char	*modify_time;
}				t_data;

struct			s_node
{
	void			*comparing_data;
	char			*name;
	t_data			*data;
	t_node			*left;
	t_node			*right;
	unsigned char	height;
};

typedef struct      s_init
{
	unsigned char   flag;
	char            **args;
	int				(*comparing_func)(void *, t_data *);
	int				(*print_func)(t_node *);
	t_node			*head;
}                   t_init;

void	parse_input(int ac, char **av, t_init *init);
void	select_compare_function(t_init *init);
t_node	*balance(t_node *node);
int		sort_by_name(void *name, t_data *data);
int		sort_by_name_rev(void *name, t_data *data);
t_node	*insert_node(t_init *init, t_node *node, t_data *data);

#endif //LS_LS_H
