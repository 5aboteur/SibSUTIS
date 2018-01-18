#include <stdio.h>
int main()
{
	int mas[100][100], n, m, i, j, a;
	printf("Input n:");
	scanf("%d", &n);
	printf("Input m:");
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
	
	printf("Input a: ");
	scanf("%d", &a);
	
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			mas[i][j]+=a;
		}
	}
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			printf("%d ", mas[i][j]);
		}
		printf("\n");
	}
	return 0;
}
