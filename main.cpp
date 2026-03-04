#include <iostream>
#include "hanoi4.cpp"

using namespace std;

int main() {
    cout << "Enter number of rings: ";
    int n;
    cin >> n;
    hanoi4(n);

    return 0;
}