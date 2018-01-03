#include "bfs.h"

int main()
{
	int width = 1000;
	int height = 1000;
	int V = width * height;
	int vstart = 0, vend = V - 1;

	Graph g(V);

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height - 1; ++j)
		{
			g.addEdge(i + (j * width), i + (j * width) + width, 1);
			g.addEdge(j + (i * width), j + (i * width) + 1, 1);
		}
	}

	g.addEdge(8, 49, 180);

	std::cout << "Start bfs algorithm.." << std::endl;
	g.bfs(vend, vstart);
	std::cout << "Done!" << std::endl;

	getchar();
	return 0;
}
