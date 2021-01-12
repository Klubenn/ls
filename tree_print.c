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

void	select_print_function(t_node *init)
{

}
