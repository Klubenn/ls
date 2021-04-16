#include "ls.h"


/*
 * Создание новой ноды для дерева, принимает дату, заполняет имя,
 * высоту и сохраняет саму дату.
 */
t_node	*new_node(t_data *data)
{
	t_node *new;

	new = (t_node *)ft_memalloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->height = 1;
	new->data = data;
	return (new);
}

/*
 * Функция для дополнительного сравнения. Принимает на вход начальную
 * структуру, ноду после которой надо вставить новые данные, и сами данные.
 * Возвращает -1 или 1, чтобы вставить слева или справа. Необходимость в
 * использовании этой фунции появляется в том случае, когда сравнение
 * согласно флагам вернуло 0, то есть по первому признаку элементы идентичны.
 * Тогда дополнительное сравнение проводится по названию файла.
 */
int	additional_comparison(t_init *init, t_node *node, t_data *data)
{
	int result;

	if (init->flag & FLAG_r)
		result = sort_by_name_rev(node->data, data);
	else
		result = sort_by_name(node->data, data);
	return (result);
}

/*
 * Вставка новой ноды. Принимает начальную структуру, ноду, с которой
 * надо сравнить данные, и сами данные. Возвращает локальную вершину дерева
 * после перебалансировки. Для понимания, куда вставлять новую ноду, справа
 * или слева от текущей ноды, применяется функция сравнения. В зависимости
 * от её результата происходит рекурсивный вызов функции для правой или левой
 * ноды. Если на вход пришёл нул, то создаётся новая нода и это является
 * условием остановки рекурсии. Затем предыдущему вызову функции возвращается
 * новая нода, которая присваивается правой или левой ветви вышестоящей ноды.
 * После этого происходит перебалансировка локального дерева, которая
 * возвращает новую вершину локального дерева для очередного предыдущего
 * вызова данной фунции. Так продолжается до самой вершины, в результате
 * чего в функцию, которая первой вызвала данную функцию (collect_data)
 * возвращается новый указатель на голову всего дерева.
 */
t_node 	*insert_node(t_init *init, t_node *node, t_data *data)
{
	int result;

	if (!node)
		return (new_node(data));
	result = init->comparing_func(node->data, data);
	if (result == 0)
		result = additional_comparison(init, node, data);
	if (result == -1)
		node->left = insert_node(init, node->left, data);
	else if (result == 1)
		node->right = insert_node(init, node->right, data);
	return (balance(node));
}

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

void	fill_file_type(t_data *data, mode_t mode)
{
	mode &= S_IFMT;
	if (mode == S_IFREG)
		data->rights[0] = '-';
	else if (mode  == S_IFLNK)
		data->rights[0] = 'l';
	else if (mode == S_IFDIR)
		data->rights[0] = 'd';
	else if (mode == S_IFCHR)
		data->rights[0] = 'c';
	else if (mode == S_IFBLK)
		data->rights[0] = 'b';
	else if (mode == S_IFSOCK)
		data->rights[0] = 's';
	else if (mode == S_IFIFO)
		data->rights[0] = 'p';
}

void	fill_rights(t_data *data, mode_t mode, bool attr)
{
	data->rights[1] = (mode & S_IRUSR)? 'r' : '-';
	data->rights[2] = (mode & S_IWUSR)? 'w' : '-';
	data->rights[3] = (mode & S_IXUSR)? 'x' : '-';
	data->rights[4] = (mode & S_IRGRP)? 'r' : '-';
	data->rights[5] = (mode & S_IWGRP)? 'w' : '-';
	data->rights[6] = (mode & S_IXGRP)? 'x' : '-';
	data->rights[7] = (mode & S_IROTH)? 'r' : '-';
	data->rights[8] = (mode & S_IWOTH)? 'w' : '-';
	data->rights[9] = (mode & S_IXOTH)? 'x' : '-';
	data->rights[10] = attr? '@' : ' ';
	data->rights[11] = 0;
}

void	fill_data_rights(t_data *data, mode_t mode, bool attr)
{
	fill_file_type(data, mode);
	fill_rights(data, mode, attr);
}

t_data *new_data(struct dirent *elem, struct stat *buf, bool attr)
{
	t_data *data;

	data = (t_data *) ft_memalloc(sizeof(t_data));
	if (!data)
		return (NULL);
	fill_data_rights(data, buf->st_mode, attr);


	///////////////////
	return data;
}

bool list_attr(char *path)
{
	bool attr;

	attr = false;
	errno = 0;
	if (listxattr(path, NULL, 0, XATTR_NOFOLLOW) && errno == 0)
		attr = true;
	errno = 0;
	return (attr);
}

void	read_stat(t_init *init, char *path, struct dirent *elem)
{
	struct stat buf;
	t_data *data;

	if (lstat(path, &buf) == 0)
	{
		data = new_data(elem, &buf, list_attr(path));
		if (!data)
			return (myexit(init, ENOMEM));
		printf("%s %s\n", data->rights, path); //////////////////

//		init->head = insert_node(init, init->head, data);
		init->num_of_nodes += 1;
	}
	else
		printf("Error getting data for %s\n", path);
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



// gcc *.c  -L./libft -lft
int main(int argc, char **argv) {
	t_init init;


	parse_input(argc, argv, &init);
	absent_arguments(&init);
	select_compare_function(&init);
//	select_print_function(&init);

	collect_data_from_dir(&init, argv[1]);

//	analysis_ls(&init);
	return 0;
}
