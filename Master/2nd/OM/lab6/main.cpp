#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

typedef chrono::high_resolution_clock Clock;

void lepus(int *way, int *price, int i);

int main(int argc, char *argv[])
{
	int size, amount;

	/* LEPUS INIT */

	ifstream fin;
	fin.open("lepus.in");

	string field;

	fin >> size >> field;

	auto way = new int[size]();
	auto result = new int[size]();

	cout << endl << "Calculating result ..." << endl;

	/* CALCULATE RESULT */

	auto t_start = Clock::now();

	for (auto i = 0; i < size; ++i)
	{
		switch (field[i])
		{
			case '.':
				way[i] = 0;
				break;
			case '"':
				way[i] = 1;
				break;
			case 'w':
				way[i] = -1;
				break;
			default:
				break;
		}

		lepus(way, result, i);
	}

	auto t_end = Clock::now();

	/* PRINT RESULTS */

	cout << endl << "Result array: ";

	for (auto i = 0; i < size; ++i)
	{
		cout << result[i] << " ";
	}

	cout << endl << "Way: ";

	for (auto i = 0; i < size; ++i)
	{
		cout << way[i] <<  " ";
	}

	cout << endl;

	cout << endl << "Result: " << result[size - 1] << ", Time: " <<
		chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count() <<
		" ms" << endl;

	delete[] result;
	delete[] way;

	fin.close();

	return 0;
}

void lepus(int *way, int *price, int i)
{
	if (i == 0)
	{
		price[i] = 0;
	}
	else if (way[i] == -1)
	{
		price[i] = -1;
	}
	else if ((i == 3) || (i == 4))
	{
		int m = max(price[i - 1], price[i - 3]);

		if (way[i] >= 0 && m != -1)
		{
			price[i] = (m + way[i]);
		}
		else
		{
			price[i] = -1;
		}
	}
	else if (i >= 5)
	{
		int m = max(price[i - 1], price[i - 3]);
		m = max(price[i - 5], m);

		if (way[i] >= 0 && m != -1)
		{
			price[i] = (m + way[i]);
		}
		else
		{
			price[i] = -1;
		}
	}
	else
	{
		if ((way[i] >= 0) && (price[i - 1] >= 0))
		{
			price[i] = (way[i] + price[i - 1]);
		}
		else
		{
			price[i] = -1;
		}
	}
}
