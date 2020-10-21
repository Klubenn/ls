#include "ls.h"

t_node	*new_node(t_data *data)
{
	t_node *new;

	new = (t_node *)ft_memalloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->name = data->name;
	new->height = 1;
	new->data = data;
	return (new);
}

int	additional_comparison(t_init *init, t_node *node, t_data *data)
{
	int result;

	if (init->flag & flag_r)
		result = sort_by_name_rev(node->name, data);
	else
		result = sort_by_name(node->name, data);
	return (result);
}

t_node 	*insert_node(t_init *init, t_node *node, t_data *data)
{
	int result;

	if (!node)
		return (new_node(data));
	result = init->comparing_func(node->comparing_data, data);
	if (result == 0)
		result = additional_comparison(init, node, data);
	if (result == -1)
		node->left = insert_node(init, node->left, data);
	else if (result == 1)
		node->right = insert_node(init, node->right, data);
	return (balance(node))
}

void	collect_data(t_init *init, char *path)
{


	init->head = insert_node(init, init->head, data);
}

void	analysis(t_init *init)
{
	char **args;

	args = init->args;
	if (!args)
		collect_data(init, ".");
	else
	{
		while (*args)
		{
			collect_data(init, *args);
			(*args)++;
		}
	}
}

int main(int argc, char **argv) {
	t_init init;

	parse_input(argc, argv, &init);
	select_compare_function(&init);
	analysis(&init);
	return 0;
}
