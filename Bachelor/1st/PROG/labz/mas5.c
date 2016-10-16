#include <stdio.h>

int main()

{
	int mas[100], i, n, min, max;
	printf("Input: ", n);
	scanf("%d", &n);
	if ((n<=0) || (n>100))
	{
		printf("Ne verno!");
		return 0;
	} 
	for(i=0;i<n;i++)
	{
		scanf("%d",& mas[i]);
	}
	min=mas[0];
	max=mas[0];
	for(i=0;i<n;i++)
	{
		if (mas[i]>max)
		{
			max=mas[i];	
		}
		if (mas[i]<min)
		{
			min=mas[i];	
		}
	}
	printf("Min: %d", min);
	printf("Max: %d", max);

return 0;	
}
