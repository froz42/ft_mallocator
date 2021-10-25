#include <stdlib.h>
#include <string.h>

void other_func(void)
{
	void *dest = calloc(10000000, 1);

	if (!dest)
		exit(1);
	memset(dest, 0, 10000000);
	free(dest);
}

int main(void)
{
	void *dest = calloc(10000000, 1);
	void *src = calloc(10000000, 1);

	other_func();
	if (!src)
	{
		free(dest);
		return 1;
	}
	memcpy(dest, src, 10000000);
	free(dest);
	free(src);
	return 0;
}
