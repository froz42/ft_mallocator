#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int other_func(void)
{
	void *dest = calloc(10000000, 1);

	if (!dest)
		return (1);
	memset(dest, 0, 10000000);
	free(dest);
	return (0);
}

int main(void)
{
	void *dest = calloc(10000000, 1);
	void *src = calloc(10000000, 1);

	if (!src)
	{
		free(dest);
		return 1;
	}
	if (!dest)
	{
		free(src);
		return 1;
	}
	if (other_func())
	{
		free(src);
		free(dest);
		return 1;
	}
	memcpy(dest, src, 10000000);
	free(dest);
	free(src);
	printf("log\n");
	return 0;
}
