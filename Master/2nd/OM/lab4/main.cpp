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

	/* KNAPSACK INIT */

	ifstream fin;
	fin.open("knapsack.in");

	fin >> size >> amount;

	auto weights = new int[amount + 1]();
	auto prices = new int*[size + 1]();

	for (auto i = 0; i <= size; ++i)
	{
		prices[i] = new int[amount + 1]();
	}

	cout << endl << "Calculating maximum weight to fill the knapsack ..." << endl << endl;

	/* CALCULATE PRICES */

	auto t_start = Clock::now();

	for (auto i = 1; i <= amount; ++i)
	{
		fin >> weights[i - 1];

		for (auto j = 1; j <= size; ++j)
		{
			if (weights[i - 1] <= j)
			{
				prices[j][i] = max(prices[j][i - 1], prices[j - weights[i - 1]][i - 1] + weights[i - 1]);
			}
			else
			{
				prices[j][i] = prices[j][i - 1];
			}
		}
	}

	auto t_end = Clock::now();

	/* PRINT RESULTS */

	for (auto i = 1; i <= amount; ++i)
	{
		for (auto j = 1; j <= size; ++j)
		{
			cout << prices[j][i] << " ";
		}
		cout << endl;
	}

	cout << endl << "Total price: " << prices[size][amount] << ", Time: " <<
		chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count() <<
		" ms" << endl;

	for (auto i = 0; i <= size; ++i)
	{
		delete[] prices[i];
	}

	delete[] weights;
	delete[] prices;

	return 0;
}
