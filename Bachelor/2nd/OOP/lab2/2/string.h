#ifndef STRING_H
#define STRING_H

#include <iostream>
#include <string.h>

using namespace std;


class String {

// interface
public:

	// default constructor
	String ( int len ) {
		len = length + 1;
		pstr = new char[length];
		pstr[length] = '\0';
		for ( int i = 0; i < length - 1; i++ )
			pstr[i] = 0;
		count++;
	}

	// copy constructor
	String ( const String &copy ) {	
		length = copy.length;
		pstr = new char[copy.length];
		for ( int i = 0; i < (copy.length); i++)
			pstr[i] = copy.pstr[i];
		count++;
	}

	// constructor, which init object by array
	String ( char* str, int len ) {
		length = len + 1;
		pstr = new char[length];
		pstr[length] = '\0';
		for ( int i = 0; i < length - 1; i++ )
			pstr[i] = str[i];
		count++;
	}

	// destructor
	~String() {delete[] pstr;}

	// printing string
	void printArray() {
		for ( int i = 0; i < length; i++ ) {
			if ( length == 0 )
				cout << "This is an example string!" << endl;
			else 
				cout << pstr[i];
		}
		cout << "\n";
	}

	// reversing array
	char backWardArray() {
		int j; 
		char temp;
		for ( j = 0; j < ( length/2 ); j++ ) {
			temp = pstr[j];
			pstr[j] = pstr[length - j - 1];
			pstr[length - j - 1] = temp;
		}
		return *pstr;
	}

	// returns numbers of created objects 
	static void returnNumbersOfStrings() {
		cout << "\n" << "Numbers of Strings: "<< count << "\n" << endl;
	}

	// search symbol in the string
	void findSymb( char s ) {
		int *buf = new int[10];
		int j = 0;
		buf[0] = -1;
		for ( int i = 0; i < length; i++ ) {
	    	if ( s == pstr[i] ) {
				buf[j] = i + 1;
				j++;
			}
		}
		if ( buf[0] != -1 ) {
			cout << "Symbol '" << s << "' is in < ";
			for ( int k = 0; k < j; k++ )
				cout << buf[k] << " ";
			cout << "> place(s)" << endl;
		}
		else 
			cout << "There is no sought-for symbol." << endl;

		delete[] buf;
		return;
	}

	void cString( string str ) {
		char *cstr = new char[str.length() + 1];
		strcpy (cstr, str.c_str());

		char *p = strtok (cstr, " ");
		while ( p != 0 ) {
			cout << p << endl;
			p = strtok (NULL, " ");
		}
		delete[] cstr;
	}

// vip :))
private:
	
	int length; // cur length of string
	char *pstr; // pointer into string
	static int count; // counting numbers of created objects

};

#endif
