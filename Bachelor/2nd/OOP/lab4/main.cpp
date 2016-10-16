#include <iostream>
#include <string.h>

using namespace std;
/*
template <typename T1, typename T2, typename T3> 
void func( T1, T2, T3 ) {
	cout << "Function 'func' was called with <T1 T2 T3> arguments" << endl;
}

typedef unsigned int UI;
template <> void func<UI>( UI, UI, UI ) {
	cout << "Function 'func' was called with <UI UI UI> arguments" << endl;
}
*/
void func( int, int ) {
	cout << "Function 'func' was called with <int int> arguments" << endl;
}

void func( int, int, int ) {
	cout << "Function 'func' was called with <int int int> arguments" << endl;
}

void func( char, char, int ) { 
	cout << "Function 'func' was called with <char char int> arguments" << endl;
}

void func( float, char, float ) { 
	cout << "Function 'func' was called with <float char float> arguments" << endl;
}

void func( double, double, double ) { 
	cout << "Function 'func' was called with <double double double> arguments" << endl;
}

void func( string, string ) { 
	cout << "Function 'func' was called with <string string> arguments" << endl;
}

void func( long, long, long ) { 
	cout << "Function 'func' was called with <long long> arguments" << endl;
}

int main() {

	int i1 = -1 , i2 = 2;
	float f1 = 1.000, f2 = 2.000;
	char c1 = 'a', c2 = 'b';
	double d1 = 3.0000, d2 = 4.0000, d3 = 5.0000;
	string s1 = "Hello", s2 = "World";
	long l1 = 124124, l2 = 131313;
//	UI u1 = 10, u2 = 20;

//	func( i1, i2 );
//	func( c1, c2 );
//	func( f2, f2, f1 );
//	func( i2, c1 );
//	func( s1, i1, c1 );
//	func( u1, u2 );
//	func( s1, s1 );
//	func( i2, c2, f1 );
//	func( u1, u2, u2 );
	func( d1, d2, i1 );
	return 0;

}
