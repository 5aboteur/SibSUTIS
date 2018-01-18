#include<stdio.h>
int main()
{
	int mas[100], z, n, p, i=2, c, y, x;
	printf("Input n: ");
	scanf("%d", &n);
	
	if ((n<=0) || (n>100))
	{
		printf("Neverniy vvod!");
		return 0;
	}
	for(z=0;z<n;z++)
	{
		mas[z]=z+1;
	}	
	
	for(p=1;p*p<=n;p++)
	{
		x=p+1;
		if (mas[x]>0)
		{
			while(p*i<n)
			{
				c=p*i;
				mas[c]=0;
				i++;
			}
		}
	}
	
	for(y=1;y<n;y++)
	{
		if (mas[y]>0)
		{
			printf("%d ", mas[y]);
		}
	
	}
	return 0;	
}	
	
