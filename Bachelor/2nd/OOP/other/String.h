#ifndef STRING_H
#define STRING_H

#include <iostream>
#include <string>
using namespace std;

class String
{
	public:
	
	String(int sz);
	String(char *string, int sz);
	String(const String &rhs);
	~String() {delete[] pstr;}
	
	int size() {return _size;};
	
	void findsb(char sb);
	void findst(const String &rhs);
	
	char cstring(char *cstr);
	
	bool operator == (const String &rhs);
	bool operator != (const String &rhs);
	char& operator [] (int index);
	String& operator = (const String &rhs);
	
	private:
	
	int _size;
	char *pstr;
};

#endif

String::String (int sz)
{
	_size = sz + 1;
	pstr = new char[_size];
	pstr[_size] = '\0';

	for (int ix = 0; ix < (_size - 1); ++ix)
		pstr[ix] = 0;
	
}

String::String (char *string, int sz)
{
	_size = sz + 1;
	pstr = new char[_size];
	pstr[_size] = '\0';

	for (int ix = 0; ix < (_size - 1); ++ix)
		pstr[ix] = string[ix];
}

String::String (const String &rhs)
{
	_size = rhs._size;
	pstr = new char[_size];

	for (int ix = 0; ix < _size; ++ix)
		pstr[ix] = rhs.pstr[ix];
}

bool String::operator == (const String &rhs)
{
	if (_size != rhs._size)
		return false;
	for (int i = 0; i < _size; i++) {
		if (pstr[i] != rhs.pstr[i])
			return false;
	}
	return true;
}

bool String::operator != (const String &rhs)
{
	return !(*this == rhs);
}

char& String::operator [] (int index)
{
	return pstr[index];
}

String& String::operator = (const String &rhs)
{
	if (&rhs != this) {
		if(_size != rhs._size) {
			delete [] pstr;
			_size = rhs._size; 
			pstr = new char [_size];
		}
	}
	for (int i = 0; i < _size; i++)
		pstr[i] = rhs.pstr[i];
	return *this;
}

void String::findsb(char sb)
{
	for (int i = 0; i < _size; i++) {
		if (sb == pstr[i]){
			cout << "Symbol found at position " << i+1 << "." << endl;
			return;
		}
	}
	cout << "Symbol not found." << endl;
	return;
}

void String::findst(const String &rhs)
{
	if (_size < rhs._size) {
		cout << "Not found" << endl;
		return;
	}
	for (int i = 0; i < _size; i++) {
		if (pstr[i] == rhs.pstr[0]){
			int flag = 0;
			for(int j = 0; ((j < (rhs._size - 1)) && ((i+j) < _size) && (flag != 1)); j++) {
				if (pstr[i+j] != rhs.pstr[j])
					flag = 1;
			}
			if(flag != 1) {
				cout << "Found!" << endl;
				return;
			}
		}
	}
	cout << "Not found" << endl;
	return;
}

char String::cstring(char *cstr)
{
	for (int i = 0; i < _size; i++)
		cstr[i] = pstr[i];
	return *cstr;
}
