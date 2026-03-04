#include <iostream>
#include <vector>
#include <random>
#include "quicksort.cpp"
using namespace std;

int arr_size = 5000000;

int main() {
    // 1. Obtain a random seed from the hardware
    std::random_device rd; 
    // 2. Initialize the engine (Mersenne Twister is a common choice)
    std::mt19937 gen(rd());
    // 3. Define the distribution (range [1, 100] inclusive)
    std::uniform_int_distribution<> distrib(0, arr_size * 10);
    // 4. Generate and print a random number

    vector<int> arr;

    for (int i = 0; i < arr_size; i++){
        arr.push_back(distrib(gen));
    }

    cout << "Sorting " << arr_size << " numbers" << endl;
    quicksort(arr);
    is_sorted(arr);

    return 0;
}
