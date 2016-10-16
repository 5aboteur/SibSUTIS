#include <stdio.h>
#include <math.h>
int main()
{
	int x, y, z;
	printf("Input x, y: ");
	scanf("%d %d", &x, &y);
	if( x > y ){
		z = x;
	} else {
		z = y;
	}
	printf("min(%d, %d) = %d\n", x, y, z);
	return 0;
}
