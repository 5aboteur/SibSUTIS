#include <stdio.h>

int main()

{

int n, xn=1, fx=1, i=1;
float x;

printf("Input x ");
scanf("%f", &x);
printf("Input n ");
scanf("%d", &n);

	while (i<n)
	
	{
	
	xn=xn*x;
	fx=fx+xn;
	i++;
	
	}
	
printf("fx= %f", fx);
return 0;

}
