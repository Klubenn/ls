#include "ls.h"

/*
 * Сортировка по имени(от меньшего к большему). Принимает название файла и дата-структуру ноды дерева.
 * If new data is bigger than node should be listed after, returns 1 - вставить правее (не сортировать)
 * If equal - returns 0
 * If new data is smaller, returns -1 - вставить левее (сортировать)
 */
int	sort_by_name(t_data *old_data, t_data *new_data)
{
	int res;

	res = ft_strcmp(new_data->name, old_data->name);
	if (res > 0)
		return (1);
	if (res < 0)
		return (-1);
	return (0);
}

int	sort_by_name_rev(t_data *old_data, t_data *new_data)
{
	return (-(sort_by_name(old_data, new_data)));
}

int sort_by_modification_time(t_data *old_data, t_data *new_data)
{}

int sort_by_modification_time_rev(t_data *old_data, t_data *new_data)
{
	return (-(sort_by_modification_time(old_data, new_data)));
}

int no_sort(t_data *old_data, t_data *new_data)
{
	return (1);
}
/*
 * Определение принципа сортировки дерева, в котором будет храниться
 * содержимое выводимой директории. Принимает начальную струкруру, в
 * которой хранится базовая информация. Принцип сортировки определяется
 * в зависимости от флагов. В структуру вносится адрес выбранной функции
 * сортировки.
 */
void	select_compare_function(t_init *init)
{
	if (init->flag & FLAG_f)
		init->comparing_func = &no_sort;
	else if (!(init->flag & FLAG_t) && !(init->flag & FLAG_r))
		init->comparing_func = &sort_by_name;
	else if (!(init->flag & FLAG_t) && (init->flag & FLAG_r))
		init->comparing_func = &sort_by_name_rev;
	else if ((init->flag & FLAG_t) && !(init->flag & FLAG_r))
		init->comparing_func = &sort_by_modification_time;
	else if ((init->flag & FLAG_t) && (init->flag & FLAG_r))
		init->comparing_func = &sort_by_modification_time_rev;
}

