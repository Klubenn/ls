#include "../includes/ls.h"

void	*free_data(t_data *data)
{
	if (!data)
		return (NULL);
	free(data->name);
	free(data->path);
	free(data->user);
	free(data->group);
	free(data->link_to_file);
	free(data);
	return (NULL);
}

void	free_node(t_node *node)
{
	if (!node)
		return;
	if (node->data)
		free_data(node->data);
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

void	free_dir_list(t_init *init)
{
	t_dir_list *dir_list;
	t_dir_list *prev_dir;

	dir_list = init->dir_list;
	while (dir_list)
	{
		prev_dir = dir_list;
		dir_list = dir_list->next;
		free(prev_dir->path);
		free(prev_dir);
	}
}

void	free_args(t_init *init)
{
	if (init->args_files)
		free(init->args_files);
	if (init->args_dirs)
		free(init->args_dirs);
	if (init->args_absent)
		free(init->args_absent);
}

void	free_col(t_col *col)
{
	t_col *init_col;
	t_col *prev_col;

	init_col = col;
	while(1)
	{
		free(col->string);
		prev_col = col;
		col = col->next;
		free(prev_col);
		if (col == init_col)
			break;
	}
}

void	myexit(t_init *init, int err)
{
	if (init)
	{
		free_args(init);
		if (init->head)
			free_tree(init->head);
		if (init->col)
			free_col(init->col);
	}
	exit(err);
}