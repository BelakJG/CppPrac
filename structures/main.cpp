#include <iostream>
#include "MyVector.hpp"
using namespace std;

int main() {
    MyVector<int> testArr(32);
    for (int i = 0; i < 32; ++i) {
        testArr.push_back(i);
    }
    for (int i = 0; i < 32; ++i) {
        cout << testArr[i] << " ";
    }
    cout << endl;

    return 0;
}