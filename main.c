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

void	add_element_to_dir_list(t_init *init, t_node *node)
{
	t_dir_list *current_dir;

	if (!node->data->)//todo добавить проверку на директорию
		return;
	current_dir = (t_dir_list *)ft_memalloc(sizeof(t_dir_list));
	if (!current_dir)
		return (myexit(init, ENOMEM));
	current_dir->next = init->dir_list;
	init->dir_list = current_dir;
	current_dir->data = copy_t_data(node->data);
	if (!current_dir->data)
		return (myexit(init, ENOMEM));
}

void	collect_dirs_from_tree(t_init *init)
{
	apply_infix(init, init->head, &add_element_to_dir_list);
	///
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
	struct stat buf;
	t_data *data;

	if (!path)
		return;
	d = opendir(path);
	if (d)
	{
		while ((elem = readdir(d)) != NULL)
		{
			if (stat(elem->d_name, &buf) == 0)
			{
				data = new_data(elem, buf);
				init->head = insert_node(init, init->head, data);
			}
			else
				printf("Error getting data for %s\n", elem->d_name);
		}
		closedir(d);
	}



	//
	if (init->flag & FLAG_R)
		collect_dirs_from_tree(init);
}

/*
 * Начало обработки аргументов. При их отсутствии обрабатывается
 * текущая директория. В противном случае сначала обрабатываются и
 * выводятся все файлы из поданных на вход аргументов, а затем директории.
 */
void	analysis(t_init *init)
{
	if (!init->args_files && !init->args_dirs)
	{
		collect_data_from_dir(init, ".");
		apply_infix(init, init->head, init->print_func);
		free_tree(init->head);
	}
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
	select_print_function(&init);
	analysis(&init);
	return 0;
}
