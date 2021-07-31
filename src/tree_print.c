#include <sys/ioctl.h>
#include "../includes/ls.h"

bool    check_dot_dirs(char *path)
{
    char *new;

    if (!(new = ft_strrchr(path, '/')))
        new = path;
    if (ft_strcmp(".", new) == 0 || ft_strcmp("..", new) == 0)
        return (true);
    return false;

}

/*
 * Вызов callback функции при обходе дерева слева направо. Сюда может быть
 * передана ф-ция вывода на печать, поиска директорий. Принимает (локальную)
 * вершину дерева и идёт максимально влево, печатает левый элемент, затем
 * центральный и после этого правый.
 */
void	apply_infix(t_init *init, t_node *node, void (*callback_func)(t_init *, t_node *))
{
	if (node == NULL)
		return ;
	else
	{
        if (node->left)
			apply_infix(init, node->left, callback_func);
        if (callback_func)
		    callback_func(init, node);
        else if (check_dot_dirs(node->data->name))
			add_element_to_dir_list(init, node);
		if (init->flag & FLAG_R && !check_dot_dirs(node->data->name))
            add_element_to_dir_list(init, node);
		if (node->right)
			apply_infix(init, node->right, callback_func);
	}
}

/*
 * подсчёт размеров полей для ссылок и размера файла для ровного отступа при выводе на печать
 */
void calculate_length_for_print(t_init *init)
{
    int i;

    for (i = 0; init->max_links; i++)
        init->max_links /= 10;
    init->max_links = i;
    for (i = 0; init->max_size; i++)
        init->max_size /= 10;
    init->max_size = i;
}

/*
 * подробная печать
 */
void	print_l(t_init *init, t_node *node)
{
	ft_printf("%s %*u %-*s  %-*s  ", node->data->rights,
		(u_int32_t)init->max_links, node->data->links,
		init->max_user, node->data->user,
		init->max_group, node->data->group);
	if (init->major)
		ft_printf("%s", node->data->major);
	ft_printf("%*llu %s %5s %s", (u_int32_t)init->max_size, node->data->size_minor,
		node->data->month_day,
		node->data->time_year,
		node->data->name);
	if (node->data->link_to_file)
		ft_printf(" -> %s", node->data->link_to_file);
	ft_printf("\n");
}

void	print_g(t_init *init, t_node *node)
{
	ft_printf("%s %*u %-*s  ", node->data->rights,
			  (u_int32_t)init->max_links, node->data->links,
			  init->max_group, node->data->group);
	if (init->major)
		ft_printf("%s", node->data->major);
	ft_printf("%*llu %s %5s %s", (u_int32_t)init->max_size, node->data->size_minor,
			  node->data->month_day,
			  node->data->time_year,
			  node->data->name);
	if (node->data->link_to_file)
		ft_printf(" -> %s", node->data->link_to_file);
	ft_printf("\n");
}
/*
 * краткая печать в столбик
 */
void	print_1(t_init *init, t_node *node)
{
    if (init)
		ft_printf("%s\n", node->data->name);
}

void	create_col_structs(t_init *init)
{
	t_col			*col;
	int				col_width_in_tabs;
	u_int8_t		number_of_rows;
	int				number_of_cols;
	struct winsize	ws;
	u_int8_t		string_width;
	int				i;

	col = (t_col *) ft_memalloc(sizeof(t_col));
	if (!col)
		myexit(init, ENOMEM);
	init->col = col;
	col_width_in_tabs = init->max_len / 8 + 1;
	ioctl(0, TIOCGWINSZ, &ws);
	string_width = (ws.ws_col >= init->max_len)? ws.ws_col + 1 : init->max_len + 1;
	number_of_cols = string_width / 8 / col_width_in_tabs;
	if (number_of_cols == 0)
		number_of_cols = 1;
	number_of_rows = (init->num_of_nodes % number_of_cols) ?  (init->num_of_nodes / number_of_cols + 1) :
					(init->num_of_nodes / number_of_cols);
	for (i = 0; i < number_of_rows; i++)
	{
		col->sequence_number = i;
		col->amount_of_tabs = col_width_in_tabs;
		col->string = (char *) ft_memalloc(ws.ws_col + 1);
		col->next = (i == number_of_rows - 1)? init->col : (t_col *) ft_memalloc(sizeof(t_col));
		if (!col->next)
			myexit(init, ENOMEM);
		col = col->next;
	}
}

void	collect_col(t_init *init, t_node *node)
{
	t_col *col;
	int sequence_number;
	int tabs_to_print;

	if (!init->col)
		create_col_structs(init);
	col = init->col;
	while(col->sequence_number < col->next->sequence_number)
		col = col->next;
	sequence_number = col->sequence_number + 1;
	col = col->next;
	if (col->last_elem_len)
	{
		tabs_to_print = 0;
		while(col->last_elem_len + tabs_to_print * 8 < col->amount_of_tabs * 8)
		{
			ft_strcat(col->string, "\t");
			tabs_to_print += 1;
		}
	}
	ft_strcat(col->string, node->data->name);
	col->last_elem_len = (int)ft_strlen(node->data->name);
	col->sequence_number = sequence_number;
}

void	print_col(t_init *init)
{
	t_col *col;

	col = init->col;
	if (!col)
		return;
	while(1)
	{
		ft_printf("%s\n", col->string);
		col = col->next;
		if (col == init->col)
			break;
	}
	free_col(init);
}

void    print_dir(t_init *init, char *path)
{
    calculate_length_for_print(init);
    if (init->print_path || init->print_line)
		ft_printf("%s:\n", path);
    if (init->flag & FLAG_l && init->head)
		ft_printf("total %llu\n", init->total_for_dir);
    apply_infix(init, init->head, init->print_func);
    if (init->print_func == &collect_col)
    	print_col(init);
}

/*
 * выбор функции вывода на печать
 */
void	select_print_function(t_init *init)
{
	init->print_func = &collect_col;
	if (init->flag & FLAG_1)
		init->print_func = &print_1;
	if (init->flag & FLAG_l)
		init->print_func = &print_l;
	if (init->flag & FLAG_g)
		init->print_func = &print_g;
}
