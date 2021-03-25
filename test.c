#include <stdio.h>
#include "ls.h"

void test_parsing(t_init *init)
{
	// test flags
	printf("%u - flags\n", init->flag);

	// test args
	char **dir = init->args_dirs;
	char **file = init->args_files;
	while(*dir)
	{
		printf("%s - directory\n", *dir);
		dir++;
	}
	while(*file)
	{
		printf("%s - file\n", *file);
		file++;
	}
}