#include "ls.h"
//todo this is not needed if in parse_args strdup is not used
//void free_args(char **args)
//{
//	int i;
//
//	i = 0;
//	while (args[i])
//	{
//		free(args[i]);
//		i++;
//	}
//	free(args);
//}

void	free_node(t_node *node)
{
	if (!node)
		return;
	if (node->data)
	{
		if (node->data->name)
			free(node->data->name);

		/////////////
		free(node->data);
	}
	free(node);
}

void	free_tree(t_node *node)
{
	if (!node)
		return;
	if (node->left)
		free_tree(node->left);
	if (node->right)
		free_tree(node->right);
	free_node(node);
}

void	myexit(t_init *input, int err)
{
	if (input)
	{
		if (input->args_files)
			free(input->args_files);
		if (input->args_dirs)
			free(input->args_dirs);
		if (input->head)
			free_tree(input->head);
	}
	exit(err);
}