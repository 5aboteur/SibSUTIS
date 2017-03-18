#include <algorithm>
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

#define Pi 3.141592654 

class cS3 {
public:
	double start_x, end_x;   
	double a0, b0, d0;
	double an, cn, dn;
	double *y;

	cS3(int _n);
	~cS3();

	void BuildS3();
	double Func(double x);

private:
	double *m, *d, *c, *b, *a, *_l, *_m; 
	double h;
	int n;
};

cS3::cS3(int _n)
{
	n = _n;
	m = new double[n + 1];
	d = new double[n + 1];
	c = new double[n + 1];
	b = new double[n + 1];
	a = new double[n + 1];
	y = new double[n + 1];
	_m = new double[n + 1];
	_l = new double[n + 1];
}

cS3::~cS3()
{
	delete m, y, _m, _l;
}

void cS3::BuildS3()
{
	h = (end_x - start_x) / n;
	a[0] = a0;
	b[0] = b0;
	c[0] = 0;
	d[0] = d0;
	a[n] = an;
	b[n] = 0;
	c[n] = cn;
	d[n] = dn;

	for (int i = 1; i < n; i++) {
		b[i] = h / 6;
		c[i] = h / 6;
		a[i] = 2 * h / 3;
		d[i] = (y[i + 1] - 2 * y[i] + y[i - 1]) / h;
	}

	_l[0] = -b[0] / a[0];
	_m[0] = d[0] / a[0];

	for (int i = 1; i <= n; i++) {
		_l[i] = -b[i] / (a[i] + c[i] * _l[i - 1]);
		_m[i] = (d[i] - c[i] * _m[i - 1]) / (a[i] + c[i] * _l[i - 1]);
	}

	m[n] = _m[n];

	for (int i = n - 1; i >= 0; i--)
		m[i] = _l[i] * m[i + 1] + _m[i];
}

double cS3::Func(double x)
{
	int i = int(((x - start_x) / h) + 1);

	if (i > n) i--;

	double xi = start_x + i*h;
	double xi_ = xi - h;

	return (xi - x)*y[i - 1] / h + (x - xi_)*y[i] / h +
		(xi - x)*((xi - x)*(xi - x) - h*h)*m[i - 1] / (6 * h) +
		(x - xi_)*((x - xi_)*(x - xi_) - h*h)*m[i] / (6 * h);
}

class cApp {
public:
	cS3 *s3;

	cApp() { s3 = NULL; }
	~cApp() { if (s3) delete s3; }

	void Build(int);
	double maxd(int);
	void Decide();
	void input();
	double A, B;
} App;

void cApp::input()
{
	cout << "Input interpolation range: " << endl;
	cout << "Start = ";
	cin >> A;
	cout << "End = ";
	cin >> B;
}

void cApp::Build(int n)
{
	double h = (B - A) / n;

	s3 = new cS3(n);
	s3->start_x = A;
	s3->end_x = B;

	for (int i = 0; i <= n; i++)
		s3->y[i] = sin(h*i);

	s3->a0 = s3->an = h / 3;
	s3->b0 = s3->cn = h / 6;
	s3->d0 = (s3->y[1] - s3->y[0]) / h - 1;
	s3->dn = -1 - (s3->y[n] - s3->y[n - 1]) / h;
	s3->BuildS3();
}

double cApp::maxd(int n)
{
	double h = (B - A) / n, maxd = 0;

	for (int i = 0; i <= n; i++)
		maxd = max(maxd, fabs(sin(i*h) - s3->Func(i*h)));

	return maxd;
}

void cApp::Decide()
{
	Build(5);

	double dmax = maxd(2000), domax, ddoc = dmax;

	cout << "+----------------------------------------------------------+" << endl;
	cout << '|' << setw(10) << "n" << '|' << setw(15) << "dmax" << '|' << setw(15) << "doc" << '|' << setw(15) << "Kd" << '|' << endl;
	cout << "+----------------------------------------------------------+" << endl;
	cout << '|' << setw(10) << 5 << '|' << setw(15) << dmax << '|' << setw(15) << '-' << '|' << setw(15) << '-' << '|' << endl;

	for (int i = 10; i < 1000000; i *= 2) {
		delete s3;
		domax = dmax;
		Build(i);
		dmax = maxd(2000);
		ddoc /= 2;
		cout << '|' << setw(10) << i << '|' << setw(15) << dmax << '|' << setw(15) << ddoc << '|' << setw(15) << domax / dmax << '|' << endl;
	}

	cout << "+----------------------------------------------------------+" << endl;
}

int main()
{
	App.input();
	App.Decide();

	return 0;
}
