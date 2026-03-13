#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "quicksort.cpp"
using namespace std;

size_t arr_size = 5000000;

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

    auto start = std::chrono::steady_clock::now();
    quicksort(arr);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    cout << "Sorting " << arr_size << " numbers took " << duration.count() / 1000.0 << " seconds" << endl;
    is_sorted(arr);

    return 0;
}
