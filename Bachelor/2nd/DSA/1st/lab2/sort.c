#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000

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
        printf("%d ", v[i]);
    }
	printf("\n");
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
    pivot = v[r];
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
    if(l < r) {
        k = partition(v, l, r);
        quicksort(v, l, k - 1);
        quicksort(v, k + 1, r);
    }
}

void selectionsort(int *v, int l, int r)
{
    int i, j;
    int min;
    for (j = l; j < r; j++) {
        min = j;
        for (i = j + 1; i < r + 1; i++) {
            if (v[i] < v[min]) {
                min = i;
            }
        }
        if (min != j) {
            swap(&v[j], &v[min]);
        }
    }
}

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec*1E3+t.tv_usec)*1E-3;
}

int main()
{
    int i;
    double t;
    fillarray(array, N);
    printarray(array, N);
    //quicksort(array, 0, N - 1);
    //for (i = 0; i < 5; i++) {
    //    fillarray(array, N); 
    //    t -= wtime(); 
    //    quicksort(array, 0, N - 1);
    //    t += wtime();
    //}
    //t = t / 5.0;
    //printf("Quicksort time: %.6f sec.\n", t);
    selectionsort(array, 0, N - 1);
    for (i = 0; i < 5; i++) {
        fillarray(array, N); 
        t -= wtime(); 
        selectionsort(array, 0, N - 1);
        t += wtime();
    }
    t = t / 5.0;
    printf("Selection sort time: %.6f sec.\n", t);
    printarray(array, N);
    //system("PAUSE");
    return 0;
}
