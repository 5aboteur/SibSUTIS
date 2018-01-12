#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#include <vector>

typedef std::pair<int, int> iPair;

struct Graph
{
	int V, E;
	std::vector<std::pair<int, iPair>> edges;

	Graph(int _v, int _e) : V(_v), E(_e) {}

	void addEdge(int _u, int _v, int _w)
	{
		edges.push_back({ _w, { _u, _v } });
	}
};

#endif
