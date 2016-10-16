#include<stdio.h>
int main()
{
	int mas[100][100], n, m, i, j, sum=0;
	printf("Input n: ");
	scanf("%d", &n);
	printf("Input m: ");
	scanf("%d", &m);
	
	if ((n<=0) || (n>100))
	{
		printf("Nedopustimoe znachenie!\n");
		return 0;
	}
	
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
			sum+=mas[i][j];
		}
	}
	printf("%d", sum);
	return 0;
}
