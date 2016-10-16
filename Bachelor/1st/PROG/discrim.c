#include <stdio.h>
int main()
{
	float a, b, c, d; 
	printf("Input a, b, c\n");
	scanf("%f %f %f" ,&a, &b, &c);
	d = b*b - (4 * a * c);
	printf("D =  %f\n", d);
	printf("D = b*b-4*a*c = %f\n", d);
	printf("D = %f-4*%f*%f=%f\n", b*b, a, c, d);
	return 0;
}