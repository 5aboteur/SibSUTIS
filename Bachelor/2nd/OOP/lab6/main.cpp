#include <iterator>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

namespace std {
	istream& operator >> (istream& is, pair<string, string>& ps) { return is >> ps.first >> ps.second; }
	ostream& operator << (ostream& os, const pair<const string, string>& ps) { return os << ps.first << " | " << ps.second; }
}

class Functor {
public:
	Functor(std::vector<std::string>& vec) : _vec(vec) {}

	void operator() (const std::pair<std::string, std::string> &p) {
		std::replace(_vec.begin(), _vec.end(), p.first, p.second);
	}
private:
	std::vector<std::string>& _vec;
};


int main() {

	// input files
	std::ifstream first("file1.txt");
	std::ifstream second("file2.txt");
	std::ofstream third("file3.txt");

	// read file to string (by chars)
	std::string f1((std::istreambuf_iterator<char>(first)), (std::istreambuf_iterator<char>()));
	std::string f2((std::istreambuf_iterator<char>(second)), (std::istreambuf_iterator<char>())); 

	// string's istream
	std::istringstream is1(f1);
	std::istringstream is2(f2); 

	// define map & vector
	std::map<std::string, std::string> map;
	std::vector<std::string> vec;
	std::vector<std::string>::iterator it = vec.begin();

	// map & vector insert iterators
	std::insert_iterator< std::map<std::string, std::string> > mit(map, map.begin());
	std::insert_iterator<std::vector<std::string> > vit(vec, vec.begin());

	// end of stream
	const std::istream_iterator<std::pair<std::string, std::string> > eos1; 
	const std::istream_iterator<std::string> eos2;

	// istream iterator (from beginning)
	std::istream_iterator<std::pair<std::string, std::string> > its1(is1);
	std::istream_iterator<std::string> its2(is2);

	// copy from start to end into map & vector
	std::copy(its1, eos1, mit);
	std::copy(its2, eos2, vit);

	// looking for a same words in map and vector
	std::for_each(map.begin(), map.end(), Functor(vec));

	// copy to output
	std::cout << "-----------------MAP:" << std::endl << std::endl;
	std::copy(map.begin(), map.end(), std::ostream_iterator<std::pair<std::string, std::string> >(std::cout, "\n"));
	std::cout << std::endl;
	std::cout << "-----------------VECTOR:" << std::endl << std::endl;
	std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(std::cout, " "));
	std::cout << std::endl;

	// vector to file
	std::ostream_iterator<std::string> output_iterator(third, " ");
	std::copy(vec.begin(), vec.end(), output_iterator);
	
	getchar();
	return 0;
}
