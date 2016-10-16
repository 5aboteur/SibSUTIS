#include <iostream>
#include "string.h"

using namespace std;

int String::count = 0;

int main()
{

	char z = 'z';
	char str[] = {'a','z','a','z','a','a','5','3','1'};

	String::returnNumbersOfStrings();
	String str2Mas(str, 9);
	String str3Mas = str2Mas;

	cout << "azazaa531" << endl;
	str2Mas.findSymb(z);
	cout << endl;
	cout << "Init default string:" << endl;
	String strMas(1);
	cout << "Init your string:" << endl;
	str2Mas.printArray();
	cout << "Copy your string into new string:" << endl;
	str3Mas.printArray();

	String::returnNumbersOfStrings();

	cout << "Let's reverse your string:" << endl;
	str2Mas.backWardArray();
	str2Mas.printArray();

	return 0;

}
