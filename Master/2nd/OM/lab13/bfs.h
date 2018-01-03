#pragma once

#include <ctime>
#include <iostream>
#include <list>
#include <vector>

class Graph
{
public:
	Graph(int V);

	void addEdge(int v, int w, int wgt);
	void bfs(int s, int t);
	void displayPath(int f);

private:
	int V;
	int *prev;

	std::vector<int> * adj;
	std::vector<int> * wgt;
};
