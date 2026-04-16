#include <vector>
#include <array>
#include <stack>
#include <cmath>
#include <iostream>
#include "insertionSort.cpp"
#include "heapsort.cpp"

using namespace std;

void is_sorted(vector<int> &arr) {
    int end = arr.size() - 1;
    for (int i = 0; i < end; ++i) {
        if (arr[i] > arr[i+1]) {
            cout << "Array not sorted" << endl;
            return;
        }
    }
    cout << "array Sorted" << endl;
}

void introsortQueue(vector<int> &arr) {
    if (arr.empty()) return;

    int left = 0;
    int right = int(arr.size() - 1);
    int max_depth = log2(arr.size()) * 2;

    stack<array<int, 3>> stack;
    array<int, 3> segment = {left, right, max_depth};
    stack.push(segment);

    while (!stack.empty()) {
        segment = stack.top();
        stack.pop();
        
        left = segment[0];
        right = segment[1];
        max_depth = segment[2];

        while (left < right) {
            if (right - left <= 16) {
                insertionSort(arr, left, right);
                break;
            }
            if (max_depth <= 0) {
                heapsort(arr, left, right);
                break;
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
                    ++i;
                } while (arr[i] <= pivot);
                do {
                    --j;
                } while (arr[j] >= pivot);
                if (i >= j) {
                    break;
                }
                swap(arr[i], arr[j]);
            }

            if (j - left < right - j) {
                stack.push({left, j, max_depth - 1});
                left = j + 1;
            } else {
                stack.push({j+1, right, max_depth - 1});
                right = j;
            }
            --max_depth;
        }
    }
}