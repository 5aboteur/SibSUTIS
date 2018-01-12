#include <algorithm>
#include "graph.h"
#include "disjointset.h"

int kruskalMST(Graph g);

int main(void)
{
	int V = 9, E = 14;
	Graph g(V, E);

	g.addEdge(0, 1, 4);
	g.addEdge(0, 7, 8);
	g.addEdge(1, 2, 8);
	g.addEdge(1, 7, 11);
	g.addEdge(2, 3, 7);
	g.addEdge(2, 8, 2);
	g.addEdge(2, 5, 4);
	g.addEdge(3, 4, 9);
	g.addEdge(3, 5, 14);
	g.addEdge(4, 5, 10);
	g.addEdge(5, 6, 2);
	g.addEdge(6, 7, 1);
	g.addEdge(6, 8, 6);
	g.addEdge(7, 8, 7);

	std::cout << "Edges of MST are: \n\n";

	int mst_wt = kruskalMST(g);

	std::cout << "\nWeight of MST is " << mst_wt << std::endl;

	getchar();
	return 0;
}

int kruskalMST(Graph g)
{
	int mst_wt = 0;

	std::sort(g.edges.begin(), g.edges.end());

	DisjointSet ds(g.V);

	for (auto it = g.edges.begin(); it != g.edges.end(); it++)
	{
		int u = it->second.first;
		int v = it->second.second;

		int set_u = ds.find(u);
		int set_v = ds.find(v);

		if (set_u != set_v)
		{
			std::cout << " " << u << " -> " << v << std::endl;

			mst_wt += it->first;
			ds.merge(set_u, set_v);
		}
	}

	return mst_wt;
}
