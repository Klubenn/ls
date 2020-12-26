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
	new->name = data->name;
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

	if (init->flag & flag_r)
		result = sort_by_name_rev(node->name, data);
	else
		result = sort_by_name(node->name, data);
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
	result = init->comparing_func(node->comparing_data, data);
	if (result == 0)
		result = additional_comparison(init, node, data);
	if (result == -1)
		node->left = insert_node(init, node->left, data);
	else if (result == 1)
		node->right = insert_node(init, node->right, data);
	return (balance(node))
}

/*
 * Здесь должны будут собираться все данные по файлу или содержимому
 * директории по заданному пути, создаваться структура дата, вставляться
 * в дерево. Принимает на вход начальную структуру и путь.
 */
void	collect_data(t_init *init, char *path)
{


	init->head = insert_node(init, init->head, data);
}

/*
 * Начало обработки аргументов. При их отсутствии обрабатывается
 * текущая директория. В противном случае аргументы обрабатываются
 * в порядке поступления.
 */
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
	//todo вставить сортировку аргументов-путей на файлы и директории
	analysis(&init);
	return 0;
}
