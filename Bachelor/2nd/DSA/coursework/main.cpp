#include "kmeans.hpp"

int main(int argc, char **argv)
{
	uint ncls, ndms, npts;

	if (argc != 4) {
		std::cout << "You da pussy ass nigga! Next time put 3 on it, faggit."
			<< std::endl << "Executing program with default parameters "
			<< "(number of clusters: 3, number of dimensions: 3, number of points: 100)"
			<< std::endl << "Press enter, lil bitch." << std::endl;
		getchar();
		ncls = 3;
		ndms = 3;
		npts = 100;
	} else {
		ncls = atoi(argv[1]);
		ndms = atoi(argv[2]);
		npts = atoi(argv[3]);
		std::cout << "Number of clusters: " << ncls << ", number of dimensions: "
			<< ndms << ", number of points: " << npts << ". Press enter button.";
		getchar();
	}

	PSpace ps(npts, ndms);
	CSpace cs(ncls, ps);

	cs.k_means();

	std::cout << cs;

	return 0;
}
