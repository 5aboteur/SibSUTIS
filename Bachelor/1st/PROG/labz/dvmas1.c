#include<stdio.h>
int main()
{
	int mas[100][100], n, m, i, j;
	printf("Input n: ");
	scanf("%d", &n);
	printf("Input m: ");
	scanf("%d", &m);
	
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			scanf("%d", &mas[i][j]);
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
