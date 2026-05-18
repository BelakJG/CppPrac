#include <iostream>
#include <vector>
using namespace std;

int n = 50;
long long *fibbCache = new long long[n + 1];

long long fibb(int n) {
    if (fibbCache[n] != -1) {
        return fibbCache[n];
    }

    long long num = fibb(n - 1) + fibb(n - 2);
    fibbCache[n] = num;
    return num;
}

int main() {
    fill(fibbCache, fibbCache + n + 1, -1);
    fibbCache[0] = 0;
    fibbCache[1] = 1;
    cout << fibb(n) << endl;

    delete[] fibbCache;
    return 0;
}