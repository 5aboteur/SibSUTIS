#include <stdio.h>
int main()
{
	float xn, e, x, n, i, en; // en = fact, xn = x*x
	printf("Input n, x\n");
	scanf("%f %f",&n ,&x);
	e=1;
	en=1;
	i=1;
	xn=x;
	while (n>0)
	{
		en=en*i;
		e=e+(xn/en);
		n--;
		i++;
		xn=xn*x;		
	}
	printf("e^x= %f",e);
	return 0;
}