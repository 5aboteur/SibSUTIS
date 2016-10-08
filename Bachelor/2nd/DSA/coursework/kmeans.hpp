#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>

typedef unsigned int		uint;
typedef unsigned short		ushort;

typedef std::set<uint>		setpoints;
typedef std::vector<setpoints>	ctp;
typedef std::vector<uint>	ptc;
typedef std::vector<double>	point;
typedef std::vector<point>	points;

std::ostream& operator << (std::ostream& os, point& _p);
std::ostream& operator << (std::ostream& os, points& _ps);
std::ostream& operator << (std::ostream& os, setpoints& _s);
std::ostream& operator << (std::ostream& os, ctp& _ctp);
std::ostream& operator << (std::ostream& os, ptc& _ptc);

double distance(const point& a, const point& b);

class PSpace {
	private:
		uint	npts;
		uint	ndims;
		points	bucket;

		void init_points();

	public:
		PSpace(uint _npoints, uint _ndimensions) :
			npts(_npoints), ndims(_ndimensions) { init_points(); };

		inline const uint get_num_of_points() const { return npts; }
		inline const uint get_num_of_dimensions() const { return ndims; }
		inline const point& get_point(uint pid) const { return bucket[pid]; }

		friend std::ostream& operator << (std::ostream& os, PSpace& ps)
		{
			uint pid = 0;

			for (auto& it : ps.bucket)
				os << "Point[" << pid++ << "]={" << it << std::endl;

			return os;
		}
};

class CSpace {
	private:
		uint	nclus;
		uint	ndims;
		uint	npts;
		ctp	ctop;
		ptc	ptoc;

		points	ncds;
		PSpace&	ps;

		void compute_centroids();
		void init_partition();

	public:
		CSpace(uint _nclusters, PSpace& _ps) :
			nclus(_nclusters), ps(_ps), ndims(_ps.get_num_of_dimensions()),
			npts(_ps.get_num_of_points()), ptoc(npts, 0) {
				for (uint i = 0; i < nclus; ++i) {
					point p;

					for (uint d = 0; d < ndims; ++d)
						p.push_back(0.0);

					setpoints setp;
					ncds.push_back(p);
					ctop.push_back(setp);
				}
		}

		void k_means();

		friend std::ostream& operator << (std::ostream& os, CSpace& cs)
		{
			uint cid = 0;

			for (auto& cit : cs.ctop) {
				os << std::endl << "Cluster[" << cid++ << "]={" << std::endl << " ";

				ushort frmt = 0;

				for (auto& sit : cit) {
					point p = cs.ps.get_point(sit);
					os << "(" << p << ")";

					if (!(++frmt % 10)) {
						os << std::endl << " ";
						frmt = 0;
					}
				}

				os << std::endl << "}" << std::endl;
			}

			return os;
		}
};


#endif

