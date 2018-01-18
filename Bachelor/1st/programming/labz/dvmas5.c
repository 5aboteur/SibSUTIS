#include<stdio.h>
int main ()
{
	int mas[100][100], n, m, i, j, min=0, max=0;
	printf("Input n: ");
	scanf("%d", &n);
	printf("Input m: ");
	scanf("%d", &m);
	
	if ((n<=0) || (n>100))
	{
		printf("Nevernoe znachenie!");
		return 0;
	}
	
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			scanf("%d", &mas[i][j]);
		}
	}
	
	min=mas[0][0];
	max=mas[0][0];
	
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{	
			if (mas[i][j]<min)
			{
			min=mas[i][j];
			}
			if (mas[i][j]>max)
			{
				max=mas[i][j];
			}
		}
	}
	printf("min= %d\n", min);
	printf("max= %d", max);
	return 0;
}
	
