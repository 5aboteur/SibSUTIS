#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <math.h>

#define INF 9999

using namespace std;

void func(int mas[8][8], int price[8][8], int i, int j) {
    if ((i == 0) && (j == 0))
        price[i][j] = 0;
    else if ((i == 0) && (j != 0))
        price[0][j] = price[0][j - 1] + mas[0][j];
    else if ((i != 0) && (j == 0))
        price[i][0] = price[i - 1][0] + mas[i][0];
    else {
        int m = min(price[i - 1][j], price[i][j - 1]);
        m = min(m, price[i - 1][j - 1]);
        price[i][j] = m + mas[i][j];
    }
}


int main() {
    ifstream fin("king2.in");
    ofstream fout("king2.out");

    int mas[8][8];
    int rmas[8][8];
    int price[8][8];

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            fin >> mas[7 - i][j];
    }

    ////////////////////////////////
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            func(mas, price, i, j);
    }

    fout << price[7][7];

    fin.close();
    fout.close();

    //system("PAUSE");
    return 0;
}
