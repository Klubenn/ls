#include <time.h>
# include "ls.h"

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

unsigned int module(time_t t1, time_t t2)
{
	long long res;

	res = t1 - t2;
	if (res >= 0)
		return (res);
	return (-res);
}

void	fill_time(t_data *data, struct timespec time_file)
{
	char *t;
	bool over_six_months;
	time_t time_now;

	time_now = time(NULL);
	over_six_months = module(time_now, time_file.tv_sec) > SIX_MONTHS ? true : false;
	t = ctime(&(time_file.tv_sec));
	printf("%s\n", t);
	ft_strncpy(data->month_day, ft_strchr(t, ' ') + 1, 6);
	if (over_six_months)
		ft_strncpy(data->time_year, ft_strrchr(t, ' ') + 1, 4);
	else
		ft_strncpy(data->time_year, ft_strchr(t, ':') - 2, 5);
}

int	fill_ownership(t_init *init, t_data *data, uid_t uid, gid_t gid)
{
	struct passwd	*pwd;
	struct group	*grp;

	pwd = getpwuid(uid);
	grp = getgrgid(gid);
	if (!pwd || !grp)
		return (-1);
	if (!(data->user = ft_strdup(pwd->pw_name)))
		return (-1);
	if (!(data->group = ft_strdup(grp->gr_name)))
		return (-1);
	init->max_user = MAX(ft_strlen(data->user), init->max_user);
	init->max_group = MAX(ft_strlen(data->group), init->max_group);
	return (0);
}

t_data *new_data(t_init *init, struct dirent *elem, struct stat *buf, bool attr)
{
	t_data *data;

	data = (t_data *) ft_memalloc(sizeof(t_data));
	if (!data)
		return (NULL);
	if (!(data->name = ft_strdup(elem->d_name)))
	{
		free(data);
		return (NULL);
	}
	if (init->flag & FLAG_l)
	{
		fill_data_rights(data, buf->st_mode, attr);
		data->blocks = buf->st_blocks;
		init->total_for_dir += data->blocks;
		data->links = buf->st_nlink;
		init->max_links = (init->max_links >= data->links)? init->max_links : data->links;
		fill_time(data, buf->st_mtimespec);
		if (fill_ownership(init, data, buf->st_uid, buf->st_gid) != 0)
		{
			free(data);
			return (NULL);
		}
		data->size = buf->st_size;
		init->max_size = MAX(data->size, init->max_size);
	}
	if (init->flag & FLAG_t)
		data->time = (double)buf->st_mtimespec.tv_sec + ((double)buf->st_mtimespec.tv_nsec / 1000000000LU);





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
		if (elem->d_name[0] == '.' && !(init->flag & FLAG_a))
			return;
		data = new_data(init, elem, &buf, list_attr(path));
		if (!data)
			return (myexit(init, ENOMEM));
		init->head = insert_node(init, init->head, data);
		init->num_of_nodes += 1;
	}
	else
		printf("Error getting data for %s\n", path);
}
