#include<stdio.h>
int main()
{
	int n, a[100], i, sum=0; // k-vremya do nuzhnogo klienta
	int c;
	printf("Vvod n: ");
	scanf("%d", &n);
	if ((n<=0) && (n>100))
	{
		printf("Neverniy vvod");
		return 0;
	}
	for(i=0;i<n;i++)
	{
		scanf("%d", &a[i]);
	}
	
	printf("Want enter client # (0/1): ");
	scanf("%d",&c);
	
	while( c == 1 ){
		printf("Enter client #: ");
		scanf("%d",&k);
		for
			sum+=a[i];
		printf("%d", sum);
		
		printf("Enter client # (0/1): ");
		scanf("%d",&c);
	}
	return 0;
}
