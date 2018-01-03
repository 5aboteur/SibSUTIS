#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

#define ITERS 100000

double drand(double min, double max)
{
	double num = min + static_cast <double> (rand()) / (RAND_MAX / (max - min));
	return num;
}

double f1(double x, double y)
{
	return (1.5 - x + x * y) * (1.5 - x + x * y) + (2.25 - x + x * y * y) * (2.25 - x + x * y * y) + (2.625 - x + x * y * y * y) * (2.625 - x + x * y * y * y);
}

double x_partial(double x, double y)
{
	double x_prime = (f1(x + 0.000001, y) - f1(x - 0.000001, y)) / 0.000002;
	return x_prime;
}

double y_partial(double x, double y)
{
	double y_prime = (f1(x, y + 0.000001) - f1(x, y - 0.000001)) / 0.000002;
	return y_prime;
}

double isd()
{
	srand(static_cast <unsigned> (time(0)));

	double tol = 0.0000001;
	double const alpha = 1.1;
	double const beta = 0.5;
	double ds = 0.5;
	double x, y, grad, gradx, grady, coeff;
	double dx, dy;
	double last_fit, fit;
	double low = -4.5;
	double high = 4.5;
	double x0 = drand(low, high);
	double y0 = drand(low, high);
	bool constraint = true;

	last_fit = f1(x0, y0);
	fit = last_fit;

	for (int i = 0; i < ITERS; ++i)
	{
		gradx = -1 * x_partial(x0, y0);
		grady = -1 * y_partial(x0, y0);
		grad = std::sqrt(gradx * gradx + grady * grady);

		if (grad == 0)
		{
			return fit;
		}

		coeff = ds / grad;

		x = x0 + coeff * gradx;
		y = y0 + coeff * grady;

		if (x < low || x > high || y < low || y > high)
		{
			constraint = false;
		}

		fit = f1(x, y);

		if (std::abs(fit - last_fit) <= tol)
		{
			return fit;
		}

		dx = x - x0;
		dy = y - y0;

		if (std::abs(dx) <= tol && std::abs(dy) <= tol)
		{
			return fit;
		}

		if (fit > last_fit || !constraint)
		{
			ds *= beta;
		}
		else
		{
			ds *= alpha;
			last_fit = fit;
			x0 = x;
			y0 = y;
		}
	}

	return fit;
}

int main()
{
	srand(static_cast <unsigned> (time(0)));

	const int exp = 10000;
	std::vector<double> mins;
	double best = 100000;
	double avg = 0.0;

	std::clock_t start = std::clock();

	for (int i = 0; i < exp; ++i)
	{
		mins.push_back(isd());
	}

	auto avg_time = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	for (int i = 0; i < exp; ++i)
	{
		best = best < mins[i] ? best : mins[i];
		avg += mins[i];
	}

	avg /= exp;

	std::cout << "Experiments: " << exp << "\n";
	std::cout << "Best min value: " << best << "\n";
	std::cout << "Avg min value:  " << avg << "\n";
	std::cout << "Total computation time: " << avg_time << " sec\n";

	getchar();
	return 0;
}
