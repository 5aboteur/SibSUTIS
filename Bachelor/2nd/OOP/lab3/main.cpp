#include <iostream>
#include "template.h"

using namespace std;

myException ex1, ex2, ex3, ex4;


int main() 
{
	ex1.type = "Exception code 1: Uncorrect index.";
	ex2.type = "Exception code 2: Size more than 1.000.000.";
	ex3.type = "Exception code 3: Different sizes.";
	ex4.type = "Exception code 4: Wrong allocation.";
	
	const int array_size = 4;
	//const int bad_size = 10;
	
	
	try {
		Array<int> ia(array_size);
		Array<int> i2a(array_size);
		Array<float> fa(array_size);
		Array<double> da(array_size);
		Array<char> ca(array_size);

	for ( int i = 0; i < array_size; i++ ) {
		ia[i] = i;
		i2a[i] = i * 3;
		fa[i] = i * 1.1345;
		da[i] = i * 3.5786;
		ca[i] = i + 'a';
	}
	//ia = i2a;
	cout << endl;

	cout << "{#1:} operator[]" << endl;
	cout << "ia: ";
	for ( int i = 0; i < array_size; i++ ) {
		cout << ia[i] << " ";
	}
	cout << endl;
	cout << "i2a: ";
	for ( int i = 0; i < array_size; i++ ) {
		cout << i2a[i] << " ";
	}
	cout << endl;
	cout << "fa: ";
		for ( int i = 0; i < array_size; i++ ) {
		cout << fa[i] << " ";
	}
	cout << endl;
	cout << "da: ";
	for ( int i = 0; i < array_size; i++ ) {
		cout << da[i] << " ";
	}
	cout << endl;
	cout << "ca: ";
	for ( int i = 0; i < array_size; i++ ) {
		cout << ca[i] << " ";
	}
	cout << endl;
	cout << endl;

	cout << "{#2:} swap_x()" << endl;
	ia.swap_x(i2a);
	cout << "ia now: ";
	for ( int i = 0; i < array_size; i++ ) {
		cout << ia[i] << " ";
	}
	cout << endl;
	cout << "i2a now: ";
	for ( int i = 0; i < array_size; i++ ) {
		cout << i2a[i] << " ";
	}
	cout << endl;
	cout << endl;

	cout << "{#3:} operators<< & operator>>" << endl;
	cout << "ia: " << ia;
	cout << "Input fa: ";
	cin >> fa;
	cout << "fa: " << fa << endl;

	cout << "{#4:} operator=" << endl;
	ia = i2a;
	cout << "ia now: ";
	for ( int i = 0; i < array_size; i++ ) {
		cout << ia[i] << " ";
	}
	cout << endl;
	cout << "i2a now: ";
	for ( int i = 0; i < array_size; i++ ) {
		cout << i2a[i] << " ";
	}
	cout << endl;
	cout << endl;

	cout << "{#5:} operator== & operator!=" << endl;
	if ( ( ia == i2a ) == true )
		printf("ia == i2a");
	else printf("ia != i2a");
	cout << endl;

	} catch( myException ex ) {
		ex.what();
	}
	return 0;
}
