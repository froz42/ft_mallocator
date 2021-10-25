#include <stdlib.h>

int main(void)
{
	void *dest = malloc(10000000);
	free(dest);
	dest = malloc(10);
	free(dest);
	dest = malloc(10000);
	free(dest);
	int i = 10;
	while (i--)
	{
		dest = malloc(10000000);
		free(dest);
	}
	return 0;
}
