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



char *join_path(char *first, char *second)
{
	size_t len_1;
	size_t len_2;
	int separator_needed;
	char *joined_str;

	separator_needed = 0;
	len_1 = ft_strlen(first);
	len_2 = ft_strlen(second);
	if (first[len_1 - 1] != '/')
		separator_needed = 1;
	joined_str = (char *)ft_memalloc(len_1 + len_2 + 1 + separator_needed);
	if (!joined_str)
		return(NULL);
	ft_memcpy(joined_str, first, len_1);
	if (separator_needed)
		joined_str[len_1] = '/';
	ft_memcpy(joined_str + len_1 + separator_needed, second, len_2);
	return (joined_str);
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
	char *joined_path;

	printf("%s\n", path);
	if (!path)
		return;
	d = opendir(path);
	if (d)
	{
		while ((elem = readdir(d)) != NULL)
		{
			joined_path = join_path(path, elem->d_name);
			if (!joined_path)
				return myexit(init, ENOMEM);
			read_stat(init, joined_path, elem->d_name);
			free(joined_path);
		}
		closedir(d);

		calculate_length_for_print(init);
		printf("total %llu\n", init->total_for_dir);////////////
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

char *get_file_name(char *path)
{
    char *new;

    if ((new = ft_strrchr(path, '/')))
        return (new + 1);
    return path;
}

void collect_elements(t_init *init, char **elements)
{
	while (*elements)
	{
        read_stat(init, *elements, *elements);
		// сбор данных по файлу/директории
		// отправка данных в дерево
		elements++;
	}
    calculate_length_for_print(init);
    apply_infix(init, init->head, init->print_func);/////////
}

/*
 * Начало обработки аргументов. При их отсутствии обрабатывается
 * текущая директория. В противном случае сначала обрабатываются и
 * выводятся все файлы из поданных на вход аргументов, а затем директории.
 */
void	analysis_ls(t_init *init)
{
	t_dir_list *list;

	if (!init->args_files && !init->args_dirs)
		collect_data_from_dir(init, ".");
	else if (init->args_files)
		collect_elements(init, init->args_files);
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
}

// gcc *.c  -L./libft -lft
int main(int argc, char **argv) {
	t_init init;

	ft_memset(&init, 0, sizeof(t_init));
	parse_input(argc, argv, &init);
	absent_arguments(&init);
	select_compare_function(&init);
	select_print_function(&init);
	analysis_ls(&init);
	return 0;

    write(1, "-\n", 2);
}

/*
ls: /Users/gtristan/.CFUserTextEncoding/: Not a directory
ft_ls: /Users/gtristan/.CFUserTextEncoding/: No such file or directory


*/