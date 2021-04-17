#include "ls.h"

/*
 * Вывод дерева на печать обходом слева направо. Принимает (локальную)
 * вершину дерева и идёт максимально влево, печатает левый элемент, затем
 * центральный и после этого правый. Принимает также функцию печати.
 */
void	apply_infix(t_init *init, t_node *node, void (*callback_func)(t_init *, t_node *))
{
	if (node == NULL)
		return ;
	else
	{
		if (node->left)
			apply_infix(init, node->left, callback_func);
		callback_func(init, node);
		if (node->right)
			apply_infix(init, node->right, callback_func);
	}
}

void	print_l(t_init *init, t_node *node)
{
	printf("%s %*u %-*s  %-*s  %*llu %s %5s %s\n", node->data->rights,
		(u_int32_t)init->max_links, node->data->links,
		init->max_user, node->data->user,
		init->max_group, node->data->group,
		(u_int32_t)init->max_size, node->data->size,
		node->data->month_day,
		node->data->time_year,
		node->data->name);
}

void	select_print_function(t_init *init)
{
	if (init->flag & FLAG_l)
		init->print_func = &print_l;
}
