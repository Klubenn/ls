#include "ls.h"

/*
 * If new data is bigger than node/should be listed after, returns 1
 * If equal - returns 0
 * If new data is smaller, returns -1
 */
int	sort_by_name(void *name, t_data *data)
{
	char *new_name;
	char *old_name;

	new_name = data->name;
	old_name = (char *)name;
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

int	sort_by_name_rev(void *name, t_data *data)
{
	return (-(sort_by_name(name, data)));
}

int sort_by_modification_time(void *time, t_data *data)
{}

int sort_by_modification_time_rev(void *time, t_data *data)
{}

void	select_compare_function(t_init *init)
{
	if (!(init->flag & flag_t) && !(init->flag & flag_r))
		init->comparing_func = &sort_by_name;
	else if (!(init->flag & flag_t) && (init->flag & flag_r))
		init->comparing_func = &sort_by_name_rev;
	else if ((init->flag & flag_t) && !(init->flag & flag_r))
		init->comparing_func = &sort_by_modification_time;
	else if ((init->flag & flag_t) && (init->flag & flag_r))
		init->comparing_func = &sort_by_modification_time_rev;
}

