#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	int i = 0, z = 0, cnt = 0, buf[10];
	char ch;
	fstream fin;

	fin.open("file1.txt", ios::out | ios::in | ios::app);
	while (fin.get(ch)) {
		if (ch == '\n') {
			buf[i] = (cnt + 1);
			i++;
			z++;
		}
		cnt++; 
	}
	buf[i] = cnt;
	
	for (i = 0; i < z; i++)
		cout << buf[i] << " ";
	cout << endl;
	fin.clear();

	fin.seekp(ios::end);
	for (i = 0; i < z; i++)
		fin << buf[i] << " ";
	fin.clear();

	return 0;
}
