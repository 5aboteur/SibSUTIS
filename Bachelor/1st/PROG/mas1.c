#include <stdio.h>

int main()

{

	int mas[100], n, i;
	printf("Input n: ");
	scanf("%d", &n);
	for (i=0;i<n;i++)
		{
			scanf("%d", &mas[i]);
		}
	for (i=0;i<n;i++)
		{	
			printf("%d ",mas[i]);
		}

	return 0;
}
		