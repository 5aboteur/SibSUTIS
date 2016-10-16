#ifndef CLASS_H
#define CLASS_H

#include <iostream>
using namespace std;


class IntArray {

// class interface
public:

	// default constructor
	IntArray( int sz = DefSize) {
		_size = sz;
		pia = new int[_size];
		for ( int i = 0; i < _size; i++ )
			pia[i] = 0;
		count++;
	}	

	// constructor that created copy of input array
	IntArray ( const IntArray &copy ) {	
		_size = copy._size;
		pia = new int[copy._size];
				for ( int i = 0; i < (copy._size); i++)
			pia[i] = copy.pia[i];
		count++;
	}

	// init constructor by input parameters
	IntArray ( int *arr, int sz ) {
		_size = sz;
		pia = new int[_size];
		for ( int i = 0; i < _size; i++ )
			pia[i] = arr[i];
		count++;
	}
	

	// destructor
	~IntArray() {delete[] pia;}

	// method, printing array
	void printArray() {
		for ( int i = 0; i < _size; i++ )
			cout << pia[i] << " ";
		cout << "\n";
	}

	// method, let you reverse your array
	int backWardArray() {
		int j, temp;
		for ( j = 0; j < (_size/2); j++ ) {
			temp = pia[j];
			pia[j] = pia[_size - j - 1];
			pia[_size - j - 1] = temp;
		}
		return *pia;
	}

	// method, returns numbers of created objects (objects of IntArray)
	static void returnNumbersOfArrays() {
		cout << "\n" << "Numbers of arrays: "<< count << "\n" << endl;
	}

// vip
private:
	
	int _size; // array size
	int *pia; // array pointer
	static int count; // array counter
	static const int DefSize = 12; // default size

};


#endif
