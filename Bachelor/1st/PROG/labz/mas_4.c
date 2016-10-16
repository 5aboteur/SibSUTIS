#include <stdio.h>

int main()
{
	int mas[100], n, i, a;
	
	printf("Input n: ");
	scanf("%d", &n);
	if ((n<=0) || (n>100))
	{
		printf("Nedopustimoe zna4enie");
		return 0;
	}
	
	for(i=0;i<n;i++)
	{
		scanf("%d", &mas[i]);
	}
	printf("Input a: ");
	scanf("%d", &a);
	
		for (i=0;i<n;i++)
		{	
		//	printf("%d ", mas[i]);
			mas[i]+=a;
		}
		for (i=0;i<n;i++)
		{
		printf(" %d ", mas[i]);
		}
	
	return 0;
}
		
