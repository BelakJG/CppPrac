#include <vector>
using namespace std;

void heapify(vector<int> &arr, int left, int n, int root_index) {
    int largest = root_index;
    int left_child = (2 * root_index) + 1;
    int right_child = (2 * root_index) + 2;

    if ((left_child < n) && (arr[left + left_child] > arr[left + largest])) {
        largest = left_child;
    }

    if ((right_child < n) && (arr[left + right_child] > arr[left + largest])) {
        largest = right_child;
    }

    if (largest != root_index) {
        swap(arr[left + largest], arr[left + root_index]);
        heapify(arr, left, n, largest);
    }
}

void heapsort(vector<int> &arr, int left = 0, int right = -1) {
    if (right == -1) {
        right = arr.size() - 1;
    }

    int n = right - left + 1;

    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, left, n, i);
    }

    for (int i = n - 1; i > 0; --i) {
        swap(arr[left], arr[left + i]);
        heapify(arr, left, i, 0);
    }
}