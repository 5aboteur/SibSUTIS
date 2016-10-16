#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	int index = 0, cnt = 0, i = 0, j = 0;
	char mas[4][10] = {"res1.txt", "res2.txt", "res3.txt","res4.txt"};
	ifstream fin[3];
	ofstream fout("result.txt");

	for (i = 0; i < 3; i++) {

		char *fname = new char[10];
		char *buff = new char[256];

		for (j = 0; mas[i][j] != '\0'; j++)
			fname[j] = mas[i][j];
		fname[j] = '\0';

		fin[i].open(fname);

		cnt = 0;
		while (!fin[i].eof()) {
			fin[i].getline(buff, 256, '\n');
			fout << buff << endl;
			cnt++;
		}
		cout << "file " << i << ": " << cnt << endl;

		delete[] fname;
		delete[] buff;
	}

	fout.close();
	//system("PAUSE");
	return 0;
}
