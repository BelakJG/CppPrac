#include <iostream>
#include <random>
#include <chrono>
#include <sstream>
#include <locale>
#include "structures/MyVector.hpp"
#include "QuickThread.cpp"
using namespace std;

size_t arr_size = 50000000;

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, arr_size * 10);

    MyVector<int> arr;

    for (int i = 0; i < arr_size; i++)
    {
        arr.push_back(distrib(gen));
    }

    auto start = std::chrono::steady_clock::now();
    multiSort(arr, 0, arr_size - 1);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    stringstream ss;
    try
    {
        ss.imbue(locale("en_US.utf8"));
    }
    catch (const runtime_error &e)
    {
        cerr << "Locale not supported: " << e.what() << endl;
    }

    ss << arr_size;
    string formatted_num = ss.str();

    cout << "Sorting " << formatted_num << " numbers took " << duration.count() / 1000.0 << " seconds" << endl;
    is_sorted(arr);

    return 0;
}
