#include <stdio.h>
int main ()
{
	float ln, x, xn, eps; // xn - chislitel
	int k, i;
	printf("Input eps, x\n");
	scanf("%f %f", &eps, &x );
	xn=x;
	i=1;
	k=-1;
	ln=0;
	while (eps>ln)
			{		
			if(k==1)
				{ 
				ln=(x+(xn/i));
				k=-1;
				}
				else
				{
				ln=(x-(xn/i));
				k=1;
				}
			xn=xn*x;
			i++;
			}
	printf("ln(1-x)= %f", ln);
	return 0;
}
