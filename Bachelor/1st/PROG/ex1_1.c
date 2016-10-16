#include <stdio.h>
int main()
{
	float xn, s, sum, x, i, j, fact, eps;
	printf("Input eps, x\n");
	scanf("%f %f",&eps ,&x);
	sum=0;
	i=0;
	
	do 
	{
		xn=1;
		fact=1;
		j=0;
		
		while (j<i)
		{	
			xn=xn*x;
			fact=fact*j;
			j++;
		}
		
		sum+=(xn/fact);
		s=(xn/fact);
		s=s-(s<0)*2s;
		i++;
	}	while (s>=eps);
	
	printf("sum= %f",sum);	
	return 0;
}