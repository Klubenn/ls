#include "ls.h"

/*
 * возвращает текущую высоту ноды, если она существует, и 0, если нет
 */
unsigned char	height(t_node *node)
{
	return (node ? node->height : 0);
}

/*
 * возвращает разницу в высоте между левой и правой нодой
 */
int balance_factor(t_node *node)
{
	return (height(node->right) - height(node->left));
}

/*
 * Корректировка высоты ноды. Сравнивается высота левой и правой ноды и
 * берётся большая из них и к этому значению добавляется единица.
 */
void fix_height(t_node *node)
{
	unsigned char left;
	unsigned char right;

	left = height(node->left);
	right = height(node->right);
	node->height = (left > right) ? left : right;
	node->height += 1;
}

/*
 * Вращение вершины вправо. Новой вершиной становится левая нода от текущей.
 * Помимо этого корректируется левая нода для старой вершины и правая нода для
 * новой вершины. Это одна и та же нода - та, что была правой нодой у бывшей
 * левой ноды. Возвращает новую вершину.
 */
t_node *rotate_right(t_node *node)
{
	t_node *left;

	left = node->left;
	node->left = left->right;
	left->right = node;
	fix_height(node);
	fix_height(left);
	return (left);
}

/*
 * Вращение вершины влево. Всё аналогично вращению вправо, но в
 * противоположном направлении. Возвращается та нода, которая была
 * справа от текущей.
 */
t_node *rotate_left(t_node *node)
{
	t_node *right;

	right = node->right;
	node->right = right->left;
	right->left = node;
	fix_height(node);
	fix_height(right);

	return (right);
}

/*
 * Балансировка дерева от конкретной вершины. На входе сама нода, которую
 * надо проверить на равновесие. При разнице
 */
t_node	*balance(t_node *node)
{
	fix_height(node);
	if (balance_factor(node) == 2)
	{
		if (balance_factor(node->right) < 0)
			node->right = rotate_right(node->right);
		return (rotate_left(node));
	}
	if (balance_factor(node) == -2)
	{
		if (balance_factor(node->left) > 0)
			node->left = rotate_left(node->left);
		return (rotate_right(node));
	}
	return (node);
}
