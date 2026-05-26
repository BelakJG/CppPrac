#include <thread>
#include <utility>
#include <cstddef>
#include <functional>
#include "structures/MyVector.hpp"
#include "introsortQueue.cpp"
using namespace std;

void worker(MyVector<int> &arr, size_t left, size_t right) {
    introsortQueue(arr, left, right);
}

void multiSort(MyVector<int> &arr) {
    size_t left = 0;
    size_t right = arr.size() - 1;
    size_t mid = left + ((right - left) / 2);

    if (arr[left] > arr[mid]) swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) swap(arr[left], arr[right]);
    if (arr[mid] > arr[right]) swap(arr[mid], arr[right]);

    int pivot = arr[mid];
    size_t i = left - 1;
    size_t j = right + 1;
    while (true) {
        do {
            ++i;
        } while (arr[i] < pivot);
        do {
            --j;
        } while (arr[j] > pivot);
        if (i >= j) break;
        swap(arr[i], arr[j]);
    }

    thread t1(worker, ref(arr), left, j);
    introsortQueue(arr, j+1, right);

    if (t1.joinable()) t1.join();
}
