#include <stdio.h>
#include <math.h>
int main()
{
	int x, y, t;
	printf("Input x, y: ");
	scanf("%d %d", &x, &y);
	printf("min(%d, %d) = ", x, y);
	if( x > y ){
		x = y;
		t = x;
		y = t;
	}
	printf("x=%d, y=%d\n", x, y);
	return 0;
}
