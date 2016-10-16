#ifndef TEMP_H
#define TEMP_H

#include <iostream>
using namespace std;


class myException : public exception {
public:
	char* type;
	bool operator== ( myException &ex ) {
		if ( this->type == ex.type ) return true; 
		return false;
	}
	
	virtual void what() {
		cout << type << endl;
	}
};

template <class elemType>
class Array {

// interface
public:

	// default constructor
	Array ( int sz = DefSize ) {
		if ( sz > 1000000 )
			throw ex2;
		_size = sz;
		pia = new elemType[_size];
		if ( pia == NULL )
			throw ex4;
		for ( int i = 0; i < _size; i++ )
			pia[i] = 0;
	}

	// copy constructor
	Array ( const Array &copy ) {	
		_size = copy._size;
		pia = new elemType[copy._size];
		if ( pia == NULL )
			throw ex4;
		for ( int i = 0; i < (copy._size); i++)
			pia[i] = copy.pia[i];
	}

	// constructor, which init object by array
	Array ( const elemType *arr, int sz ) {
		_size = sz;
		pia = new elemType[_size];
		if ( pia == NULL )
			throw ex4;
		for ( int i = 0; i < _size; i++ )
			pia[i] = arr[i];
	}

	// destructor
	~Array() { delete[] pia; }

	// return current size of array
	int size() const { return _size; }

	// operator[] overload
	elemType &operator[] ( int a ) { 
		if ( a > this->_size )
			throw ex1;
		return this->pia[a]; 
	}

	// operator= overload
	Array &operator= ( const Array &obj ) {
		if ( this->_size != obj._size )
			throw ex3;
        if ( this != &obj ) {		
			elemType *newArr = new elemType[obj._size];
			if ( newArr == NULL )
				throw ex4;
            copy( obj.pia, obj.pia + obj._size, newArr );
            delete[] this->pia;

			this->pia = new elemType[obj._size];
			for ( int i = 0; i < this->_size; i++ )
				this->pia[i] = newArr[i];
            this->_size = obj._size;
        }
        return *this;
    }

	// operator== overload
	bool operator== ( const Array& rhs ) { 
		if ( this->_size != rhs._size ) return false;
		for ( int i = 0; i < this->_size; i++ ) 
			if ( this->pia[i] != rhs.pia[i] ) return false;
		return true;
	}

	// operator!= overload
	bool operator!= ( const Array& rhs ) { 
		return !( bool operator== ( const Array& rhs ) ); 
	}

	// operator>> overload
	friend istream& operator>> ( istream& is, const Array<elemType>& obj) {
		for ( int i = 0; i < obj._size; i++ ) 
			 is >> obj.pia[i];
		return is;
	}

	// operator<< overload
	friend ostream& operator<< ( ostream& os, const Array<elemType>& obj) {
		for ( int i = 0; i < obj._size; i++ ) 
			 os << obj.pia[i] << " ";
		os << endl;
		return os;
	}

	// printing array
	void print_array() {
		for ( int i = 0; i < _size; i++ )
			cout << pia[i] << " ";
		cout << "\n";
	}

	// swap
	void swap_x( Array<elemType>& secondary ) {
		elemType temp;
		for ( int i = 0; i < this->_size; i++ ) {
			temp = this->pia[i];
			this->pia[i] = secondary.pia[i];
			secondary.pia[i] = temp;
		}
	}

// vip :))
private:
	
	int _size; // cur size of array
	elemType *pia; // pointer into array 
	static const int DefSize = 12; // def size of array
	
};

#endif