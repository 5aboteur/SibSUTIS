#include<stdio.h>
int main()
{
	int maxi[2][4], mini[2][4], j=0, i, a[4][4]={{44, 52, 16, 15},
							   	      		     {50, 16, 29, 38},
								    			 {11, 49, 92, 80},
								    			 {75, 15, 10, 25}};
	for(i=0;i<4;i++)
	{
		maxi[0][i]=a[i][j];
		maxi[1][i]=j;
		for(j=0;j<3;j++)
		{
			if(maxi[0][i]<a[i][j])
			{
				maxi[0][i]=a[i][j];
				maxi[1][i]=j;
			}
		}
	}
	for(i=0;i<2;i++)
	{
		for(j=0;j<4;j++)
		{
			printf("%d ", maxi[i][j]);
		}
		printf("\n");
	}
	// minimum poshel
	for(j=0;j<4;j++)
	{
		mini[0][j]=a[i][j];
		mini[1][j]=i;
		for(i=0;i<4;i++)
		{
			if(mini[0][i]>a[i][j])
			{
				mini[0][j]=a[i][j];
				mini[1][j]=i;
			}
		}
	}
	for(i=0;i<2;i++)
	{
		for(j=0;j<4;j++)
		{
			printf("%d ", mini[i][j]);
		}
		printf("\n");
	}
	return 0;
}
