#include <iostream>
#include <climits>

int MatrixChainOrder(int p[], int n)
{
	auto **m = new long long*[n];
	for (int i = 0; i < n; ++i)
	{
		m[i] = new long long[n];
	}

	for (int i = 1; i < n; ++i)
	{
		m[i][i] = 0L;
	}

	for (int L = 2; L < n; ++L)
	{
		for (int i = 1; i < n - L + 1; ++i)
		{
			int j = i + L - 1;

			m[i][j] = LLONG_MAX;

			for (int k = i; k <= j - 1; ++k)
			{
				long long q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];

				if (q < m[i][j])
				{
					m[i][j] = q;
				}
			}
		}
	}

	auto result = m[1][n - 1];

	for (int i = 0; i < n; ++i)
	{
		delete[] m[i];
	}

	delete[] m;

	return result;
}

int main()
{
	int size = 1000;
	auto arr = new int[size];

	for (int i = 0; i < size; ++i)
	{
		arr[i] = i + 1;
	}

	std::cout << "Number of multiplications = " << MatrixChainOrder(arr, size) << std::endl;

	delete[] arr;

	getchar();
	return 0;
}
