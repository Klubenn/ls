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
	printf("%s %*u %-*s  %-*s  ", node->data->rights,
		(u_int32_t)init->max_links, node->data->links,
		init->max_user, node->data->user,
		init->max_group, node->data->group);
	if (init->major)
		printf("%s", node->data->major);
	printf("%*llu %s %5s %s", (u_int32_t)init->max_size, node->data->size_minor,
		node->data->month_day,
		node->data->time_year,
		node->data->name);
	if (node->data->link_to_file)
        printf(" -> %s", node->data->link_to_file);
    printf("\n");
}

/*
 * краткая печать в столбик
 */
void	print_1(t_init *init, t_node *node)
{
    if (init)
	    printf("%s\n", node->data->name);
}

void    print_dir(t_init *init, char *path)
{
    calculate_length_for_print(init);
    if (init->print_path || init->print_line)
        printf("%s:\n", path);
    if (init->flag & FLAG_l && init->head)
        printf("total %llu\n", init->total_for_dir);
    apply_infix(init, init->head, init->print_func);
}

/*
 * выбор функции вывода на печать
 */
void	select_print_function(t_init *init)
{
	init->print_func = &print_1;
	if (init->flag & FLAG_l)
		init->print_func = &print_l;
}
