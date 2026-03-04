#include <iterator>
#include <iostream>
#include <vector>
#include <random>
using namespace std;

void swap(vector<int> &arr, int left, int right) {
    int temp = arr[left];
    arr[left] = arr[right];
    arr[right] = temp;
}

void is_sorted(vector<int> &arr) {
    for (int i = 0; i < arr.size() - 2; i++) {
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

    int j = left - 1;
    for (int i = left; i < right; i ++) {
        if (arr[i] < arr[right]) {
            j += 1;
            swap(arr, j, i);
        }
    }

    j += 1;
    swap(arr, j, right);

    if (left < (j - 1)) {
        quicksort(arr, left, j - 1);
    }

    if (right > (j + 1)) {
        quicksort(arr, j + 1, right);
    }
}