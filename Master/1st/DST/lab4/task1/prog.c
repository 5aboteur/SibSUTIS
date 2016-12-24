#include <limits.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	for (int i = 0; i < INT_MAX; ++i);
	return 0;
}
