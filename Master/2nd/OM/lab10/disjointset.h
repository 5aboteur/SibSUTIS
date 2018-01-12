#ifndef _DISJOINTSET_H_
#define _DISJOINTSET_H_

struct DisjointSet
{
	int *parent, *rank;
	int n;

	DisjointSet(int);
	~DisjointSet();

	int find(int _u);
	void merge(int _x, int _y);
};

#endif
