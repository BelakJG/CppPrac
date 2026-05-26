#include <vector>
#include <array>
#include <stack>
#include <cmath>
#include <iostream>
#include "insertionSort.cpp"
#include "heapsort.cpp"
#include "structures/MyStack.hpp"

using namespace std;

void is_sorted(auto &arr) {
    int end = arr.size() - 1;
    for (int i = 0; i < end; ++i) {
        if (arr[i] > arr[i+1]) {
            cout << "Array not sorted" << endl;
            return;
        }
    }
    cout << "array Sorted" << endl;
}

struct Segment {
    int left;
    int right;
    int depth;
};

void introsortQueue(auto &arr, int main_left = 0, int main_right = -1) {
    if (arr.empty()) return;

    if (main_right == -1) main_right = int(arr.size() - 1);
    int max_depth = log2(arr.size()) * 2;

    MyStack<Segment> stack;
    Segment segment = {main_left, main_right, max_depth};
    stack.push(segment);

    while (!stack.empty()) {
        segment = stack.top();
        
        int left = segment.left;
        int right = segment.right;
        max_depth = segment.depth;
        stack.pop();

        while (left < right) {
            if (right - left <= 24) {
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
                } while (arr[i] < pivot);
                do {
                    --j;
                } while (arr[j] > pivot);
                if (i >= j) {
                    break;
                }
                swap(arr[i], arr[j]);
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
}