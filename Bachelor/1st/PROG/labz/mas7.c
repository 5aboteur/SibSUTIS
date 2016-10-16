#include <stdio.h>

int main()

{
	int a[100], b[100], n, i, l=0; // l = kol-vo chetnih
	printf("Input n: ");
	scanf("%d", &n);
	if ((n<=0)||(n>100))
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
		if (a[i]%2 == 0)
		{
			b[l]=i;
			l++;
		}
	}	
	for(i=0;i<l;i++)
	{
		printf("%d ", b[i]);
	}
	return 0;
}
