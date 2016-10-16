#include <stdio.h>

int main()

{
	int a[100], b[100], i, n;
	printf("Input n: ");
	scanf("%d", &n);
	if ((n<=0) || (n>100))
	{
		printf("Ne verno!");
		return 0;
	}
	
	for(i=0;i<n;i++)
	{
		scanf("%d", &a[i]);
	}
	for(i=0;i<n;i++)
	{
		scanf("%d", &b[i]);
		a[i]+=b[i];
	}
	for(i=0;i<n;i++)
	{
		printf("%d ", a[i]);
	}
	return 0;
}