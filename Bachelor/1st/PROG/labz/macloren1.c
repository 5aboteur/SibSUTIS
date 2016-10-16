#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  float xn, x, fx;
  int i=0, n;
  xn=1; 
  fx=1;

  printf("Input x: ");
  scanf("%f", &x);
  printf("Input n: ");
  scanf("%d", &n);

  if ((-1<x) && (x<1))
  {
     while(i <= n)
     {
        fx=fx+xn;
        xn=xn*x;  
        i++;
     }
   }
  printf("fx= %d\n", fx);
  system("PAUSE");	
  return 0;
}
