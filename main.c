#include <stdlib.h>
#include <string.h>

int main(void)
{
	void *dest = malloc(10000000);
	bzero(dest, 10000000);
	free(dest);
	dest = malloc(10);
	memset(dest, 0, 10);
	free(dest);
	dest = malloc(10000);
	memset(dest, 0, 10000);

	free(dest);
	int i = 10;
	while (i--)
	{
		dest = malloc(10000000);
		bzero(dest, 10000000);
		free(dest);
	}
	return 0;
}
