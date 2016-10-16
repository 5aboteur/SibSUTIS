#include <iostream>
#include <istream>
#include <fstream>

using namespace std;

istream readStream(istream &stream);

int main()
{
	readStream(cin);
	return 0;
}

istream readStream(istream &stream) 
{	
	char ch;
	
	while ((ch = stream.get()) != EOF)
		cout << ch;
		 
	stream.clear();
	return stream;
}
