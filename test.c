#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

int main(int ac, char **av)
{
	int i;
	int j;
	int dir;
	int file;
	struct stat buf;

	i = 0;
	dir = 0;
	file = 0;
	while (--ac > 0 && ++av)
	{
		if (*av && *av[0] != '-')
		{
			errno = 0;
			j = stat(*av, &buf);
			if (j == 0)
			{
				if (S_ISDIR(buf.st_mode))
					dir++;
				else
					file++;
				printf("+++%s\n", *av);
			}
			else
				printf("%s - does not exist; %d\n", *av, errno);
		}
	}
	printf("%d - files, %d - directories\n", file, dir);
	return 0;
}