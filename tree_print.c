#include "ls.h"

/*
 * Вывод дерева на печать обходом слева направо. Принимает (локальную)
 * вершину дерева и идёт максимально влево, печатает левый элемент, затем
 * центральный и после этого правый. Принимает также функцию печати.
 */
void	btree_apply_infix(t_node *node, void (*print_func)(t_node *))
{
	if (node == NULL)
		return ;
	else
	{
		if (node->left)
			btree_apply_infix(node->left, print_func);
		print_func(node);
		if (node->right)
			btree_apply_infix(node->right, print_func);
	}
}
