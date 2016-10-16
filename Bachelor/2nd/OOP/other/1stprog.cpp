#include <iostream>
#include <fstream>

using namespace std;

int main() {
		
	double a, b, c; 
	int choose;
	
	cout << "Input some shxt" << endl;
	cin >> a >> b;
	
	cout << "Choose operation" << endl;
	cout << "1 -> a - b" << endl;
	cout << "2 -> a + b" << endl;
	cout << "3 -> a / b" << endl;
	cout << "4 -> a * b" << endl;
	cin >> choose;
	switch(choose){
	case 1: c = a - b;
		cout << "res:" << c << endl;
		break;
	case 2: c = a + b;
		cout << "res:" << c << endl;
		break;
	case 3: c = a / b;
		cout << "res:" << c << endl;
		break;
	case 4: c = a * b;
		cout << "res:" << c << endl;
		break;
	}
	return 0;
} 