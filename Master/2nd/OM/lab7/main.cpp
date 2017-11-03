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
	int size;

	/* DESCENT INIT */

	ifstream fin;
	fin.open("descent.in");

	fin >> size;

	int **mountain = new int*[size];

	for (auto i = 0; i < size; ++i)
	{
		mountain[i] = new int[size];
	}

	for (auto i = 0; i < size; ++i)
	{
		for (auto j = 0; j < i + 1; ++j)
		{
			fin >> mountain[i][j];
		}
	}

	/* PRINT MOUNTAIN */

	cout << endl;

	for (auto i = 0; i < size; ++i)
	{
		for (auto j = 0; j < i + 1; ++j)
		{
			cout << mountain[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl << "Searching optimal path to get maximum points ..." << endl;

	/* FIND OPTIMAL PATH */

	auto t_start = Clock::now();

	for (auto i = size - 2; i >= 0; --i)
	{
		for (auto j = 0; j <= i; ++j)
		{
			mountain[i][j] += max(mountain[i + 1][j], mountain[i + 1][j + 1]);
		}
	}

	auto t_end = Clock::now();

	/* PRINT RESULTS */

	for (auto i = 0; i < size; ++i)
	{
		for (auto j = 0; j < i + 1; ++j)
		{
			cout << mountain[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl << "Total points: " << mountain[0][0] << ", Time: " <<
		chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count() <<
		" ms" << endl;

	for (auto i = 0; i < size; ++i)
	{
		delete[] mountain[i];
	}

	delete[] mountain;

	return 0;
}
