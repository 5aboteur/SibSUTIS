#include <cstdio>
#include <climits>
#include <ctime>
#include <iostream>

#include "graph.h"
#include "heap.h"

void dijkstra(struct Graph* graph, int src, int dst);

int main()
{
	int width = 1000;
	int height = 1000;
	int V = width * height;
	auto graph = createGraph(V);

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height - 1; ++j)
		{
			addEdge(graph, i + (j * width), i + (j * width) + width, 1);
			addEdge(graph, j + (i * width), j + (i * width) + 1, 1);
		}
	}

	addEdge(graph, 8, 49, 180);
	std::cout << "Start dijkstra algorithm.." << std::endl;
	dijkstra(graph, 0, V - 1);
	std::cout << "Done!" << std::endl;

	getchar();
	return 0;
}

void dijkstra(struct Graph* graph, int src, int dst)
{
	auto V = graph->V;
	auto dist = new int[V];
	auto minHeap = createMinHeap(V);

	for (int v = 0; v < V; ++v)
	{
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, dist[v]);
		minHeap->pos[v] = v;
	}

	minHeap->array[src] = newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);
	minHeap->size = V;

	clock_t timer = clock();

	while (!isEmpty(minHeap))
	{
		auto minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v;
		auto pCrawl = graph->array[u].head;

		while (pCrawl != NULL)
		{
			int v = pCrawl->dest;

			if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v])
			{
				dist[v] = dist[u] + pCrawl->weight;
				decreaseKey(minHeap, v, dist[v]);
			}

			pCrawl = pCrawl->next;
		}
	}

	printf("result = %d\n", dist[dst]);
	std::cout << "Elapsed time: " << (clock() - timer) / CLOCKS_PER_SEC << "s" << std::endl;

	delete[] dist;
}
