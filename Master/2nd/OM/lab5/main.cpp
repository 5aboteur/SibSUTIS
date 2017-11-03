#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef chrono::high_resolution_clock Clock;

int main(int argc, char *argv[])
{
	int size, amount;

	/* LADDER INIT */

	ifstream fin;
	fin.open("ladder.in");

	fin >> size;

	auto result = new int[size]();
	auto stairs_values = new int[size]();

	cout << endl << "Calculating maximum sum ..." << endl;

	/* CALCULATE SUM */

	auto t_start = Clock::now();

	for (auto i = 0; i < size; ++i)
	{
		fin >> stairs_values[i];

		if (i == 0)
		{
			result[i] = stairs_values[i];
		}
		else if (i == 1)
		{
			if (result[i - 1] > 0)
			{
				result[i] = stairs_values[i] + result[i - 1];
			}
			else
			{
				result[i] = stairs_values[i];
			}
		}
		else
		{
			result[i] = stairs_values[i] + max(result[i - 1], result[i - 2]);
		}
	}

	auto t_end = Clock::now();

	/* PRINT RESULTS */

	cout << endl << "Result sums: ";

	for (auto i = 0; i < size; ++i)
	{
		cout << result[i] << " ";
	}

	cout << endl << "Stairs values: ";

	for (auto i = 0; i < size; ++i)
	{
		cout << stairs_values[i] <<  " ";
	}

	cout << endl;

	cout << endl << "Result sum: " << result[size - 1] << ", Time: " <<
		chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count() <<
		" ms" << endl;

	delete[] result;
	delete[] stairs_values;

	return 0;
}
