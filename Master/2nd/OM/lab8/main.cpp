#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef chrono::high_resolution_clock Clock;

double dijkstra(vector <vector<pair<int, int>> > g, int *shortest_path, int size, int from, int to);

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

	/* PRINT ALL WAYS */

	cout << endl;

	for (auto i = 0; i < g.size(); ++i)
	{
		cout << "(" << i << ") ~> ";

		for (auto j = 0; j < g[i].size(); ++j)
		{
			cout << "(" << g[i][j].first << ") = " << g[i][j].second << ", ";
		}

		cout << endl;
	}

	cout << endl << "Searching shortest path from (" << from << ") to (" <<
		to << ") ...";

	/* FIND SHORTEST PATH */

	int shortest_path[size];

	auto t_start = Clock::now();
	auto shortest_distance = dijkstra(g, shortest_path, size, from, to);
	auto t_end = Clock::now();

	/* PRINT RESULTS */

	cout << endl << "Shortest path:";

	for (auto i = 0; i < size; ++i)
	{
		cout << " " << shortest_path[i];
	}

	cout << endl << "Distance: " << shortest_distance << ", Time: " <<
		chrono::duration_cast<chrono::nanoseconds>(t_end - t_start).count() <<
		" ns" << endl;

	return 0;
}

double dijkstra(vector <vector<pair<int, int>> > g, int *shortest_path, int size, int from, int to)
{
	auto shortest_distance = 0.0;
	auto min_dist_idx = INT_MAX;
	vector<pair<int, int>> way(size);

	for (auto i = 0; i < size; ++i)
	{
		if (i == from)
		{
			way.push_back(make_pair(0, 1));
			continue;
		}

		way.push_back(make_pair(INT_MAX, 1));
	}

	for (auto i = 0; i < size; ++i)
		cout << endl << way[i].first << " " << way[i].second << endl;

	for (auto i = 0; i < size; ++i)
	{
/*		auto min_dist = g[from][0];
		auto min_dist_idx = 0;

		for (auto j = 1; j < g[from].size(); ++j)
		{
			if (way[i].second && g[from][j].second < min_dist)
			{
				min_dist = g[from][j].second;
				min_dist_idx = j;
			}
		}

		shortest_distance += min_dist;
		way[from].second = false;
		from = 
*/

		for (auto j = 0; j < size; ++j)
		{
			if (way[j].second)
			{
				way[j].first = min(way[j].first, way[from].first + g[from][j].second); // ?
				min_dist_idx = min(way[j].first, min_dist_idx);
			}
		}

		way[from].second = false;
		from = min_dist_idx;
	}

	return shortest_distance;
}
