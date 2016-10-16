/*
 * sort.c:
 *
 */
#include <stdio.h>
#include <stdlib.h>

#define N 16

int array[N] = {2, 8, 7, 1, 3, 5, 6, 4};

void fillarray(int *v, int n)
{
    int i;
    
    for (i = 0; i < n; i++) {
        v[i] = rand() / (RAND_MAX + 1.0) * 100 + 1; 
    }
}

void printarray(int *v, int n)
{
    int i;
    
    printf("Array:\n");
    for (i = 0; i < n; i++) {
        printf("%d\n", v[i]);
    }
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int *v, int l, int r)
{
    int i, j, pivot;

    /* Выбираем опорный элемент */
    pivot = v[r];
    
    /* Переставляем элементы */
    i = l - 1;
    for (j = l; j < r; j++) {
        if (v[j] <= pivot) {
            i++;
            swap(&v[i], &v[j]);
        }
    }
    swap(&v[i + 1], &v[r]);
    return i + 1;
}

void quicksort(int *v, int l, int r)
{
    int k;
    
    if (l < r) {
        k = partition(v, l, r);
        quicksort(v, l, k - 1);
        quicksort(v, k + 1, r);
    }
}

void selectionsort(int *v, int l, int r)
{
    int i,j;
    int iMin;
    
    for (j = 0; j < n-1; j++) 
    {
		iMin = j;
        for ( i = j+1; i < n; i++)
        {
        	if (a[i] < a[iMin]) 
        	{
            	iMin = i;
            }                                                            
            if ( iMin != j ) 
            {
            swap(a[j], a[iMin]);
            }                                                                                  
        }
    }
}

/* int i;
double t;
t = 0.0;

for(i=0;i<5;i++)
{
	fillarray(array, N); 
	t -= wtime(); 
	quicksort(array, 0, N - 1);
	t+= wtime();
}
	t=t/5.0;
	printf("Quicksort time: %.6f sec.\n",t);
*/
int main()
{
    fillarray(array, N);
    printarray(array, N);
    
    quicksort(array, 0, N - 1);

    printarray(array, N);
    return 0;
}

