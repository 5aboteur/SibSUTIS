#include "bfs.h"

Graph::Graph(int V)
{
	this->V = V;
	prev = new int[V];
	adj = new std::vector<int>[V];
	wgt = new std::vector<int>[V];
}

void Graph::addEdge(int v, int w, int wt)
{
	adj[v].push_back(w);
	wgt[v].push_back(wt);
	adj[w].push_back(v);
	wgt[w].push_back(wt);
}

void Graph::bfs(int s, int t)
{
	bool *visited = new bool[V];

	for (int i = 0; i < V; ++i)
	{
		visited[i] = false;
	}

	std::list<int> queue;
	int current;
	visited[s] = true;
	prev[s] = -1;
	queue.push_back(s);
	std::vector<int>::iterator i;

	clock_t timer = clock();

	while (!queue.empty())
	{
		current = queue.front();
		queue.pop_front();

		for (i = adj[current].begin(); i != adj[current].end(); ++i)
		{
			if (!visited[*i])
			{
				prev[*i] = current;
				visited[*i] = true;

				if (*i == t)
				{
					// path was found
					std::cout << "Elapsed time: " << (clock() - timer) / CLOCKS_PER_SEC << "s" << std::endl;
					return;
				}

				queue.push_back(*i);
			}
		}
	}
}

void Graph::displayPath(int f)
{
	int p = f;
	int q;
	int pos;
	int pathlen = 0;

	std::cout << "Path is: ";

	while (p != -1)
	{
		std::cout << "->" << p;
		q = prev[p];

		if (q != -1)
		{
			pos = find(adj[p].begin(), adj[p].end(), q) - adj[p].begin();
			pathlen = pathlen + wgt[p].at(pos);
		}

		p = q;
	}

	std::cout << std::endl;
	std::cout << "pathlength : " << pathlen;
	std::cout << std::endl;
}