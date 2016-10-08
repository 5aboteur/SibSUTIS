#include "kmeans.hpp"

std::ostream& operator << (std::ostream& os, point& _p)
{
	for (auto& i : _p)
		os << " " << i << " ";

	return os;
}

std::ostream& operator << (std::ostream& os, points& _ps)
{
	for (auto& i : _ps)
		os << i << std::endl;

	return os;
}

std::ostream& operator << (std::ostream& os, setpoints& _s)
{
	for (auto& pid : _s)
		os << "PID=" << pid << " ";

	return os;
}

std::ostream& operator << (std::ostream& os, ctp& _ctp)
{
	uint cid = 0;

	for (auto& i : _ctp)
		os << "CID[" << cid++ << "]={" << i << "}" << std::endl;

	return os;
}

std::ostream& operator << (std::ostream& os, ptc& _ptc)
{
	uint pid = 0;

	for (auto& i : _ptc)
		os << "PID[" << pid++ << "]=" << i << std::endl;

	return os;
}

double distance(const point& a, const point& b)
{
	double res = 0.0;
	double diff;

	point::const_iterator cpa = a.begin();
	point::const_iterator cpb = b.begin();
	point::const_iterator cpe = a.end();

	for (; cpa != cpe; ++cpa, ++cpb) {
		diff = *cpa - *cpb;
		res += diff * diff;
	}

	return res;
}

void PSpace::init_points()
{
	for (uint pid = 0; pid < npts; ++pid) {
		point p;

		for (uint d = 0; d < ndims; ++d)
			p.push_back( rand() % 100 );

		bucket.push_back(p);

		std::cout << "PID[" << pid << "]={" << p << ")" << std::endl;
	}
}

void CSpace::init_partition()
{
	for (uint pid = 0, cid = 0; pid < ps.get_num_of_points(); ++pid) {
		cid = pid % nclus;
		ptoc[pid] = cid;
		ctop[cid].insert(pid);
	}
}

void CSpace::compute_centroids()
{
	uint cid = 0;
	uint npts = 0;

	std::cout << "Computing centroids ..." << std::endl;

	for (auto& centroid : ncds) {
		npts = 0;

		for (auto& pid : ctop[cid]) {
			point p = ps.get_point(pid);

			for (uint d = 0; d < ndims; ++d)
				centroid[d] += p[d];

			++npts;
		}

		for (uint d = 0; d < ndims; ++d)
			centroid[d] /= npts;

		++cid;
	}

	std::cout << "Centroids: " << std::endl << ncds << std::endl;
}

void CSpace::k_means()
{
	uint cid, tocl, niter = 0;
	double d, min;

	bool jopa;
	bool suka = true;

	init_partition();

	while (suka) {
		std::cout << std::endl << "Number of iterations: " << niter
			<< std::endl << std::endl;

		suka = false;

		compute_centroids();

		for (uint pid = 0; pid < npts; ++pid, ++niter) {
			min = distance(ncds[ptoc[pid]], ps.get_point(pid));
			std::cout << "PID[" << pid << "] in cluster " << ptoc[pid]
				<< " with distance: " << min << std::endl;

			cid = 0, jopa = false;

			for (auto& it : ncds) {
				d = distance(it, ps.get_point(pid));

				if (d < min) {
					min = d;
					jopa = suka = true;

					tocl = cid;
					ctop[ptoc[pid]].erase(pid);

					std::cout << "\tcluster " << cid << " closer, distance: "
						<< d << std::endl;
				}

				++cid;
			}

			if (jopa) {
				ptoc[pid] = tocl;
				ctop[tocl].insert(pid);

				std::cout << "\tmove to cluster " << tocl << std::endl;
			}
		}
	}
}
