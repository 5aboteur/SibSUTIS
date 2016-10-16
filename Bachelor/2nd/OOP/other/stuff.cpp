#include <cstdlib>
#include <iostream>
#include "String.h"
using namespace std;

int main()
{
	char ia[5] = {'p', 'u', 't', 'i', 'n'};
	char ib[7] = {'n', 'a', 'v', 'a', 'l', 'n', 'y'};
	char iaa[3] = {'u', 't', 'i'};
	char cs[6];
	
	cout << "Initializing new strings..." << endl;
	
	String A(ia, 5);
	String A1(ib, 7);
	String AA(iaa, 3);
	
	cout << "Default string A: ";
	for(int i = 0; i < 5; i++)
		cout << A[i];
	cout << endl;
	
	cout << "Default string A1: ";
	for(int i = 0; i < 7; i++)
		cout << A1[i];
	cout << endl;
	
	cout << "Size of A: " << A.size() << endl;
	
	cout << "Comparing strings..." << endl;
	
	if (A == A1) {
		cout << "Matching" << endl;
	}
	if (A != A1) {
		cout << "Non-matching" << endl;
	}
	
	cout << "Finding symbol 'a' in string A... ";
	A.findsb('a');
	cout << "...in string A1... ";
	A1.findsb('a');
	
	cout << "Finding substring 'uti' in string A... ";	
	A.findst(AA);
	
	cout << "Now, A = A1: ";
	
	A = A1;
	
	for(int i = 0; i < 7; i++)
		cout << A[i];
	cout << endl;
	
	cout << "Now, A as a C String: ";
	
	A.cstring(cs);
	
	for(int i = 0; i < 7; i++)
		cout << cs[i];
	cout << endl;
	
	cout << "Initializing empty string..." << endl;
	
	String B(5);
	
	cout << "Here it comes: ";
	for(int i = 0; i < 5; i++)
		cout << B[i] << " ";
	cout << endl;
	
	return 0;
}
