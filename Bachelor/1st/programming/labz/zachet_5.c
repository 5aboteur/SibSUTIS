#include<stdio.h>
int main()
{
	int mas[9], i, j, n, tmp, max=0;
	printf("Input n: ");
	scanf("%d", &n);
	// вводим последовательность
	for(i=0;i<n;i++)
	{
		scanf("%d", &mas[i]);
	}
	// используем метод пузырька для нахождения максимума
	for(i=0;i<n;i++)
	{
		for(j=0;j<(n-1);j++)
		{
			if(mas[j]>mas[j+1])
			{
				tmp=mas[j];
				mas[j]=mas[j+1];
				mas[j+1]=tmp;
			}
		}
	}
	printf("%d", )
