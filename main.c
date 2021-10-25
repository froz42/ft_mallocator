#include <stdlib.h>
#include <string.h>

int main(void)
{
	void *dest = calloc(10000000, 1);
	void *src = calloc(10000000, 1);

	if (!dest)
		return 1;
	if (!src)
		return 1;
	memcpy(dest, src, 10000000);
	free(dest);
	free(src);
	return 0;
}
