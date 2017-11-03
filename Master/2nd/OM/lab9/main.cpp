#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef chrono::high_resolution_clock Clock;

int dijkstra(vector <vector<pair<int, int>> >, int, int, int);

int main(int argc, char *argv[])
{
	int size, from, to;
	int v_from, v_to, weight;

	/* GRAPH INIT*/

	ifstream fin;
	fin.open("graph.in");

	fin >> size;
	fin >> from >> to;

	vector< vector<pair<int, int>> > g(size, vector<pair<int, int>>());

	while (fin >> v_from >> v_to >> weight)
	{
		g[v_from].push_back(make_pair(v_to, weight));
	}

	/* PRINT ALL distancesS */

	cout << endl;

	for (auto i = 0; i < size; ++i)
	{
		cout << "(" << i << ") > ";

		for (size_t j = 0; j < g[i].size(); ++j)
		{
			cout << "(" << g[i][j].first << ") = " << g[i][j].second << ", ";
		}

		cout << endl;
	}

	cout << endl << "Searching shortest path from (" << from << ") to (" <<
		to << ") ..." << endl;

	/* FIND SHORTEST PATH */

	auto t_start = Clock::now();
	auto shortest_distance = dijkstra(g, size, from, to);
	auto t_end = Clock::now();

	/* PRINT RESULTS */

	cout << endl << "Distance: " << shortest_distance << ", Time: " <<
		chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count() <<
		" ms" << endl;

	return 0;
}

int dijkstra(vector <vector<pair<int, int>> > g, int size, int from, int to)
{
	vector<pair<int, bool>> distances(size);

	for (auto i = 0; i < size; ++i)
	{
		if (i == from)
		{
			distances[i].first = 0;
			distances[i].second = true;
			continue;
		}

		distances[i].first = INT_MAX;
		distances[i].second = true;
	}

	for (auto i = 0; i < size; ++i)
	{
		auto min_dist_idx = -1;

		for (auto j = 0; j < size; ++j)
		{
			if (distances[j].second && (min_dist_idx == -1 || distances[j].first < distances[min_dist_idx].first))
			{
				min_dist_idx = j;
			}
		}

		if (distances[min_dist_idx].first == INT_MAX) break;

		distances[min_dist_idx].second = false;

		for (size_t j = 0; j < g[min_dist_idx].size(); ++j)
		{
			auto next_to = g[min_dist_idx][j].first;
			auto len = g[min_dist_idx][j].second;

			if (distances[min_dist_idx].first + len < distances[next_to].first)
			{
				distances[next_to].first = distances[min_dist_idx].first + len;
			}
		}
	}

	return distances[to].first;
}
