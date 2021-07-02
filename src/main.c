#include "../includes/ls.h"

/*
 * Сохранение директорий в список
 */
void	add_element_to_dir_list(t_init *init, t_node *node)
{
	t_dir_list *current_dir;

    if (node->data->rights[0] != 'd')
		return;
	current_dir = (t_dir_list *)ft_memalloc(sizeof(t_dir_list));
	if (!current_dir)
		return (myexit(init, ENOMEM));
	if (!init->dir_list)
    {
        init->dir_list = current_dir;
        init->dir_list_last_elem = current_dir;
    }
	else
    {
        init->dir_list_last_elem->next = current_dir;
        init->dir_list_last_elem = init->dir_list_last_elem->next;
    }
	current_dir->path = ft_strdup(node->data->path);
	if (!current_dir->path)
		return (myexit(init, ENOMEM));
}

/*
 * обход текущего дерева для сбора директорий в список, возвращается указатель
 * на начало списка
 */
t_dir_list	*collect_dirs_from_tree(t_init *init)
{
	apply_infix(init, init->head, NULL);
    return (init->dir_list);
}


/*
 * Соединение пути директории с именем файла и добавление разделителя '/' при необходимости
 */
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

char *return_dir_name_without_slash(char *path)
{
    char *new;

    if ((new = ft_strrchr(path, '/')))
        return (new + 1);
    return (path);
}

/*
 * Сбор содержимого директории по заданному пути, печать результата, дальнейший
 * обход при рекурсии.
 */
void	collect_data_from_dir(t_init *init, char *path)
{
	DIR *d;
	struct dirent *elem;
	char *joined_path;

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
			read_stat(init, joined_path, elem->d_name, false);
			free(joined_path);
		}
		closedir(d);
        print_dir(init, path);
        free_tree(init->head);
        init->head = NULL;
//        if (!(init->flag & FLAG_R))
//            free_dir_list(init);
	}
	else
    {
	    if (init->print_path)
            printf("%s:\n", path);
	    printf("%s: %s: Permission denied\n", init->prog_name, return_dir_name_without_slash(path));
    }
	if (init->flag & FLAG_R)
    {
        init->print_line = true;
	    process_directories(init);
    }
}

void    null_init_data(t_init *init)
{
    init->head = NULL;
    init->dir_list = NULL;
    init->dir_list_last_elem = NULL;
    init->num_of_nodes = 0;
    init->total_for_dir = 0;
    init->max_links = 0;
    init->max_user = 0;
    init->max_group = 0;
    init->max_size = 0;
}

/*
 * Обход списков директорий
 */
void    process_directories(t_init *init)
{
    t_dir_list *dir_list;
    t_dir_list *prev_dir;

    dir_list = init->dir_list;
    null_init_data(init);
    while(dir_list)
    {
        if (init->print_line)
            printf("\n");
        prev_dir = dir_list;
        collect_data_from_dir(init, dir_list->path);
        dir_list = dir_list->next;
        free(prev_dir->path);
        free(prev_dir);
        init->print_line = true;
    }
}
/*
 * Последовательнный анализ файлов или директорий
 */
void collect_elements(t_init *init, char **elements, bool files)
{
    int dirnum;

    dirnum = 0;
	while (*elements)
	{
        read_stat(init, *elements, *elements, true);
		elements++;
		dirnum++;
	}
	if (files)
    {
        calculate_length_for_print(init);
        apply_infix(init, init->head, init->print_func);
        free_tree(init->head);
        init->head = NULL;
    }
	else
    {
	    if (dirnum > 1)
	        init->print_path = true;
        collect_dirs_from_tree(init);
        free_tree(init->head);
        process_directories(init);
    }
}

/*
 * Начало обработки аргументов. При их отсутствии обрабатывается
 * текущая директория. В противном случае сначала обрабатываются и
 * выводятся все файлы из поданных на вход аргументов, а затем директории.
 */
void	select_data_for_analysis(t_init *init)
{
	if (!init->args_files && !init->args_dirs)
		collect_data_from_dir(init, ".");
    if (init->args_files && init->args_files[0])
    {
        collect_elements(init, init->args_files, true);
        init->print_line = true;
    }
    if (init->args_dirs && init->args_dirs[0])
        collect_elements(init, init->args_dirs, false);
}

int main(int argc, char **argv) {
	t_init init;

	ft_memset(&init, 0, sizeof(t_init));
	init.prog_name = argv[0];
	parse_input(argc, argv, &init);
	absent_arguments(&init);
	select_compare_function(&init);
	select_print_function(&init);
    select_data_for_analysis(&init);
    free_args(&init);
    return (0);
}
