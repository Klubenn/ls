#include "ls.h"

/*
 * Сортировка по имени. Принимает название файла и дата-структуру ноды дерева.
 * If new data is bigger than node/should be listed after, returns 1
 * If equal - returns 0
 * If new data is smaller, returns -1
 */
int	sort_by_name(t_data *old_data, t_data *new_data)
{
	char *new_name;
	char *old_name;

	new_name = new_data->name;
	old_name = old_data->name;
	while (*old_name && *new_name && *old_name != *new_name)
	{
		old_name++;
		new_name++;
	}
	if (*old_name - *new_name > 0)
		return (-1);
	if (*old_name - *new_name < 0)
		return (1);
	return (0);
}

int	sort_by_name_rev(t_data *old_data, t_data *new_data)
{
	return (-(sort_by_name(old_data, new_data)));
}

int sort_by_modification_time(t_data *old_data, t_data *new_data)
{}

int sort_by_modification_time_rev(t_data *old_data, t_data *new_data)
{}

/*
 * Определение принципа сортировки дерева, в котором будет храниться
 * содержимое выводимой директории. Принимает начальную струкруру, в
 * которой хранится базовая информация. Принцип сортировки определяется
 * в зависимости от флагов. В структуру вносится адрес выбранной функции
 * сортировки. Ничего не возвращает.
 */
void	select_compare_function(t_init *init)
{
	if (!(init->flag & FLAG_t) && !(init->flag & FLAG_r))
		init->comparing_func = &sort_by_name;
	else if (!(init->flag & FLAG_t) && (init->flag & FLAG_r))
		init->comparing_func = &sort_by_name_rev;
	else if ((init->flag & FLAG_t) && !(init->flag & FLAG_r))
		init->comparing_func = &sort_by_modification_time;
	else if ((init->flag & FLAG_t) && (init->flag & FLAG_r))
		init->comparing_func = &sort_by_modification_time_rev;
}

