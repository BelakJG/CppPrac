#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "heapsort.cpp"
using namespace std;

namespace {
    void sort(vector<int> &arr, int left, int right, int depth) {
        while (left < right) {
            if (right - left <= 16) {
                for (int i = left + 1; i <= right; ++i) {
                    int key = arr[i];
                    int j = i - 1;

                    while (j >= left && arr[j] > key) {
                        arr[j + 1] = arr[j];
                        --j;
                    }

                    arr[j + 1] = key;
                }
                return;
            }

            if (depth == 0) {
                heapsort(arr, left, right);
                return;
            }

            int mid = left + ((right - left) / 2);

            
            if (arr[mid] < arr[left]) {
                swap(arr[mid], arr[left]);
            }
            if (arr[right] < arr[left]) {
                swap(arr[right], arr[left]);
            }
            if (arr[right] < arr[mid]) {
                swap(arr[right], arr[mid]);
            }
            int pivot = arr[mid];
            int i = left - 1;
            int j = right + 1;

            while (true) {
                do {
                    i += 1;
                } while (arr[i] < pivot);

                do {
                    j -= 1;
                } while (arr[j] > pivot);

                if (i >= j) {
                    break;
                }

                swap(arr[i], arr[j]);
            }

            if (j - left < right - j) {
                sort(arr, left, j, depth - 1);
                left = j + 1;
            } else {
                sort(arr, j + 1, right, depth - 1);
                right = j;
            }
        }
    }
}

void is_sorted(vector<int> &arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            cout << "Array not sorted" << endl;
            return;
        }
    }

    cout << "Array is sorted" << endl;
}

void introsort(vector<int> &arr) {
    if (arr.size() <= 1) {
        return;
    }
    
    int left = 0;
    int right = arr.size() - 1;
    int max_depth = log2(arr.size()) * 2;
    sort(arr, left, right, max_depth);
}
