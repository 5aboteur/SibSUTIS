#include <stdio.h>
#include <math.h>

int main()
{
	float a[100][2], b[100][100];
	int i, j, n;
	printf("Input n: ");
	scanf("%d", &n);	

	if((n<=0) || (n>100))
	{
		printf("Neverniy vvod!");
		return 0;
	}
	
	for(i=0;i<n;i++)
	{
		for(j=0;j<2;j++)
		{
			scanf("%f", &a[i][j]);
		}
	}
	
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			b[i][j]=sqrt((a[i][1]-a[1][j])*(a[i][1]-a[1][j])+(a[i][2]-a[2][j])*(a[i][2]-a[2][j]));
		}
	}
	
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			printf("%f ", b[i][j]);
		}
	printf("\n");
	}
	
	
	return 0;
}
