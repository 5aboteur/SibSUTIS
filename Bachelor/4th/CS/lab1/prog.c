#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

enum vars { N = 1000000, n = 1000};

double a[4] = { 0.9, 1.0, 1.9, 2.0 }, b[4] = { 1.0, 1.1, 2.0, 2.1 };
int histogram[n] = { 0 };
double numbers[N];

int main()
{
	srand(time(0));
	
	for (int i = 0; i < N; ++i) {
		double ksi = drand48();
		if (ksi <= 0.3) {
			ksi = drand48() * (b[0] - a[0]) + a[0];
			numbers[i] = (4 - sqrt(16 - 12 * ksi)) / 2.0;
		} else if (ksi <= 1 - 0.3) {
			ksi = drand48();
			if (ksi <= 0.5) {
				ksi = drand48() * (b[1] - a[1]) + a[1];
				numbers[i] = ksi;
			} else {
				ksi = drand48() * (b[2] - a[2]) + a[2];
				numbers[i] = ksi;
			}
		} else {
			ksi = drand48() * (b[3] - a[3]) + 1.0 / 3.0;
			numbers[i] = sqrt(3 * ksi) + 1;
		}
		++histogram[int((numbers[i] * n) / 3)];
	}

	ofstream os("res.out");
	for (int i = 0; i < n; ++i) os << histogram[i] << endl;
	os.close();

	return 0;
}
