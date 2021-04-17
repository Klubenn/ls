#include "ls.h"




t_data *copy_t_data(t_data *data)
{
	t_data *new;

	new = (t_data *)ft_memalloc(sizeof(t_data));
	///
}

/*
 * Эти 2 функции нужны для рекурсивного обхода директорий
 */
void	add_element_to_dir_list(t_init *init, t_node *node)
{
	t_dir_list *current_dir;

//	if (!node->data->)//todo добавить проверку на директорию
//		return;
	current_dir = (t_dir_list *)ft_memalloc(sizeof(t_dir_list));
	if (!current_dir)
		return (myexit(init, ENOMEM));
	current_dir->next = init->dir_list;
	init->dir_list = current_dir;
	current_dir->data = copy_t_data(node->data);
	if (!current_dir->data)
		return (myexit(init, ENOMEM));
}

/*
 * обход текущего дерева для сбора директорий для рекурсии
 */
t_dir_list	*collect_dirs_from_tree(t_init *init)
{
	apply_infix(init, init->head, &add_element_to_dir_list);
	///
}



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
 * Здесь должны будут собираться все данные по файлу или содержимому
 * директории по заданному пути, создаваться структура дата, вставляться
 * в дерево. Принимает на вход начальную структуру и путь.
 */
void	collect_data_from_dir(t_init *init, char *path)
{
	DIR *d;
	struct dirent *elem;
	t_dir_list *list;
	char *join_path;

	if (!path)
		return;
	d = opendir(path);
	if (d)
	{
		while ((elem = readdir(d)) != NULL)
		{
			join_path = ft_strjoin(path, elem->d_name);
			read_stat(init, join_path, elem);
			free(join_path);
		}
		closedir(d);

		calculate_length_for_print(init);
		printf("total %lu\n", init->total_for_dir);////////////
		apply_infix(init, init->head, init->print_func);/////////
	}
	else
	{
		printf("Can't open dir %s\n", path);
	}
//	print_tree();
//	if (init->flag & FLAG_R)
//	{
//		list = collect_dirs_from_tree(init);
//		while (list)
//		{
//			collect_data_from_dir(init, list->data->name);
//			list = list->next;
//		}
//
//	}
}

void collect_elements(char **elements)
{
	while (*elements)
	{
		// сбор данных по файлу/директории
		// отправка данных в дерево
		elements++;
	}
}

/*
 * Начало обработки аргументов. При их отсутствии обрабатывается
 * текущая директория. В противном случае сначала обрабатываются и
 * выводятся все файлы из поданных на вход аргументов, а затем директории.
 */
//void	analysis_ls(t_init *init)
//{
//	t_dir_list *list;
//
//	if (!init->args_files && !init->args_dirs)
//		collect_data_from_dir(init, ".");
//	if (init->args_files)
//		collect_elements(init->args_files);
//	if (init->head)
//	{
//		print_tree();
//		free_tree();
//	}
//	if (init->args_dirs)
//	{
//		collect_elements(init->args_dirs);
//		list = collect_dirs_from_tree();
//		free_tree();
//		while (list)
//		{
//			collect_data_from_dir(init, list->data->name);
//			list = list->next;
//		}
//	}
//}

void	null_init(t_init *init)
{
	init->head = NULL;
	init->total_for_dir = 0;
	init->num_of_nodes = 0;
	init->max_links = 0;
}

// gcc *.c  -L./libft -lft
int main(int argc, char **argv) {
	t_init init;

	null_init(&init);
	parse_input(argc, argv, &init);
	absent_arguments(&init);
	select_compare_function(&init);
	select_print_function(&init);

	collect_data_from_dir(&init, argv[1]);

//	analysis_ls(&init);
	return 0;
}
