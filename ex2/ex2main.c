#include <stdio.h>
#include <limits.h>

int main () {
	long int *ptr;
	printf("%ld\n", sizeof(int));
	printf("%ld\n", sizeof(float));
	printf("%ld\n", sizeof(double));
	printf("%ld\n", sizeof(void));
	printf("%ld\n", sizeof(ptr));
	if (CHAR_MIN != 0)
		printf("Signed\n");
	else
		printf("Unsigned\n");
	return 0;
}
