#include <stdlib.h>
#include <string.h>

int main(void)
{
	void *dest = calloc(10000000, 1);

	if (dest == NULL)
	{
		return EXIT_FAILURE;
	}
	bzero(dest, 10000000);
	free(dest);
	dest = malloc(10);
	if (!dest)
		return 1;
	memset(dest, 0, 10);
	free(dest);
	dest = malloc(10000);
	if (!dest)
		return 1;
	memset(dest, 0, 10000);

	free(dest);
	int i = 10;
	while (i--)
	{
		dest = malloc(10000000);
		if (!dest)
			return 1;
		bzero(dest, 10000000);
		free(dest);
	}
	return 0;
}
