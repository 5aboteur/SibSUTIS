#include <iostream>
#include "class.h"

using namespace std;

int IntArray::count = 0;

int main()
{

	int secondaryArray[5] = {1, 2, 3, 4, 5};

	IntArray::returnNumbersOfArrays();
	

	IntArray fstMas;
	
	IntArray sndMas(secondaryArray, 5);
	IntArray trdMas = sndMas;

	cout << "Init default array:" << endl;
	fstMas.printArray();
	cout << "Init yours array:" << endl;
	sndMas.printArray();
	cout << "Copy yours array into new array:" << endl;
	trdMas.printArray();

	IntArray::returnNumbersOfArrays();

	cout << "Let's reverse our array:" << endl;
	sndMas.backWardArray();
	sndMas.printArray();
	
	return 0;
}
