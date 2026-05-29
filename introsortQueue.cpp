#include <vector>
#include <array>
#include <stack>
#include <cmath>
#include <iostream>
#include <cstdint>
#include "insertionSort.cpp"
#include "heapsort.cpp"
#include "structures/MyStack.hpp"

using namespace std;

void is_sorted(auto &arr) {
    size_t end = arr.size() - 1;
    for (size_t i = 0; i < end; ++i) {
        if (arr[i] > arr[i+1]) {
            cout << "Array not sorted" << endl;
            return;
        }
    }
    cout << "array Sorted" << endl;
}

struct Segment {
    size_t left;
    size_t right;
    size_t depth;
};

void introsortQueue(auto &arr, size_t main_left = 0, size_t main_right = SIZE_MAX) {
    if (arr.empty()) return;

    if (main_right == SIZE_MAX) main_right = int(arr.size() - 1);
    size_t max_depth = log2(arr.size()) * 2;

    MyStack<Segment> stack;
    Segment segment = {main_left, main_right, max_depth};
    stack.push(segment);

    while (!stack.empty()) {
        segment = stack.top();
        
        size_t left = segment.left;
        size_t right = segment.right;
        max_depth = segment.depth;
        stack.pop();

        while (left < right) {
            if (right - left <= 24) {
                break;
            }
            if (max_depth == 0) {
                heapsort(arr, left, right);
                break;
            }

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

            if (j - left < right - j) {
                stack.push({j + 1, right, max_depth - 1});
                right = j;
            } else {
                stack.push({left, j, max_depth - 1});
                left = j + 1;
            }
        }
    }
    insertionSort(arr, main_left, main_right);
};