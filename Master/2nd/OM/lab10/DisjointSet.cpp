#include "DisjointSet.h"

DisjointSet::DisjointSet(int _n) : n(_n)
{
	parent = new int[n + 1];
	rank = new int[n + 1];

	for (int i = 0; i <= n; ++i)
	{
		parent[i] = i;
		rank[i] = 0;
	}
}

DisjointSet::~DisjointSet()
{
	delete[] parent;
	delete[] rank;
}

int DisjointSet::find(int _u)
{
	if (_u != parent[_u])
	{
		parent[_u] = find(parent[_u]);
	}

	return parent[_u];
}

void DisjointSet::merge(int _x, int _y)
{
	_x = find(_x), _y = find(_y);

	if (rank[_x] > rank[_y])
	{
		parent[_y] = _x;
	}
	else
	{
		parent[_x] = _y;
	}

	if (rank[_x] == rank[_y])
	{
		rank[_y]++;
	}
}