#include <thread>
#include <utility>
#include <cstddef>
#include <functional>
#include <iostream>
#include "structures/MyVector.hpp"
#include "introsortQueue.cpp"
using namespace std;

int MAX_THREADS = thread::hardware_concurrency();
int threads = 1;

void multiSort(MyVector<long long> &arr, size_t left, size_t right);
void worker(MyVector<long long> &arr, size_t left, size_t right) {
    multiSort(arr, left, right);
}

void multiSort(MyVector<long long> &arr, size_t left, size_t right) {
    if (left >= right) return;
    if (right - left <= 50000) {
        introsortQueue(arr, left, right);
    } else {
        size_t mid = left + ((right - left) / 2);
        if (arr[mid] < arr[left]) swap(arr[mid], arr[left]);
        if (arr[right] < arr[left]) swap(arr[right], arr[left]);
        if (arr[right] < arr[mid]) swap(arr[right], arr[mid]);

        long long pivot = arr[mid];
        size_t i = left;
        size_t j = right;
        while (true) {
            while (arr[i] < pivot) ++i;
            while (arr[j] > pivot) --j;
            if (i >= j) break;
            swap(arr[i], arr[j]);
            ++i;
            --j;
        }

        if (right - j > j - left) {
            if (threads < MAX_THREADS) {
                ++threads;
                thread t(worker, ref(arr), j+1, right);
                multiSort(arr, left, j);
                if (t.joinable()) t.join();
                --threads;
            } else {
                multiSort(arr, left, j);
                multiSort(arr, j+1, right);
            }
        } else {
            if (threads < MAX_THREADS) {
                ++threads;
                thread t(worker, ref(arr), left, j);
                multiSort(arr, j+1, right);
                if (t.joinable()) t.join();
                --threads;
            } else {
                multiSort(arr, j+1, right);
                multiSort(arr, left, j);
            }
        }
    }
}
