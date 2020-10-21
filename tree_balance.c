#include "ls.h"

unsigned char	height(t_node *node)
{
	return (node ? node->height : 0);
}

int balance_factor(t_node *node)
{
	return (height(node->left) - height(node->right));
}

void fix_height(t_node *node)
{
	unsigned char left;
	unsigned char right;

	left = height(node->left);
	right = height(node->right);
	node->height = (left > right) ? left : right;
	node->height += 1;
}

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

t_node	*balance(t_node *node)
{
	fix_height(node);
	if (balance_factor(node) == 2)
	{
		if (balance_factor(node) < 0)
			node->right = rotate_right(node->right);
		return (rotate_left(node));
	}
	if (balance_factor(node) == -2)
	{
		if (balance_factor(node) > 0)
			node->left = rotate_left(node->left);
		return (rotate_right(node));
	}
	return (node);
}
