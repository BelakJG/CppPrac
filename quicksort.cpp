#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void is_sorted(vector<int> &arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            cout << "Array not sorted" << endl;
            return;
        }
    }

    cout << "Array is sorted" << endl;
}

void quicksort(vector<int> &arr, int left = 0, int right = -1) {
    if (right == -1) {
        right = arr.size() - 1;
    }
    if (left >= right) {
        return;
    }

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

        int middle = (left + right) / 2;
        int median_index = max(min(left, middle), min(max(left, middle), right));
        swap(arr[left], arr[median_index]);

        int pivot = arr[left];
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
            quicksort(arr, left, j);
            left = j + 1;
        } else {
            quicksort(arr, j + 1, right);
            right = j;
        }
    }
}