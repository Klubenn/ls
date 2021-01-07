#include "ls.h"

int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	else
		return (0);
}

int	ft_tolower(int c)
{
	if (c >= 65 && c <= 90)
		return (c + 32);
	else
		return (c);
}

void	ft_bzero(void *s, unsigned long n)
{
	unsigned char	*u;
	unsigned long			i;

	u = s;
	i = 0;
	if (n != 0)
	{
		while (i < n)
		{
			u[i] = 0;
			i++;
		}
		s = u;
	}
}

void	*ft_memalloc(unsigned long size)
{
	char *ptr;

	ptr = (char *)malloc(size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, size);
	return (ptr);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s1)
{
	char	*c;
	size_t	i;
	size_t	j;

	i = ft_strlen(s1);
	j = 0;
	c = (char *)malloc(sizeof(char) * (i + 1));
	if (!c || (i + 1 == 0))
		return (NULL);
	while (j < i + 1)
	{
		c[j] = s1[j];
		j++;
	}
	return (c);
}


