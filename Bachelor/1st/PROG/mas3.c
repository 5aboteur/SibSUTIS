#include <stdio.h>

int main()
{
	int mas[100], n, i, sum=0;
	
	printf("Input n: ");
	scanf("%d", &n);
	
	for (i=0;i<n;i++)
	{
		scanf("%d", &mas[i]);
	}
	
	if ((n>0) && (n<=100))
	{
		for (i=0;i<n;i++)
		{	
			printf("%d ", mas[i]);
			sum+=mas[i];
		}
		printf("Summa: %d ", sum);
	}
	else
	{
		printf("Nedopustimoe zna4enie");
	}
	return 0;
}
		