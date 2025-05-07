#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace chrono;

int comparisons = 0;
int dataMoves = 0;

void resetCounters() {
    comparisons = 0;
    dataMoves = 0;
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
        dataMoves++;
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
        dataMoves++;
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        comparisons++;
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
            dataMoves++;
        } else {
            arr[k++] = R[j++];
            dataMoves++;
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
        dataMoves++;
    }
    while (j < n2) {
        arr[k++] = R[j++];
        dataMoves++;
    }

    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void bottomUpMergeSort(int arr[], int n) {
    resetCounters();
    int* temp = new int[n];

    for (int width = 1; width < n; width *= 2) {
        for (int i = 0; i < n; i += 2 * width) {
            int left = i;
            int mid = min(i + width, n);
            int right = min(i + 2 * width, n);

            int i1 = left, i2 = mid, k = left;
            while (i1 < mid && i2 < right) {
                comparisons++;
                if (arr[i1] <= arr[i2]) {
                    temp[k++] = arr[i1++];
                    dataMoves++;
                } else {
                    temp[k++] = arr[i2++];
                    dataMoves++;
                }
            }
            while (i1 < mid) {
                temp[k++] = arr[i1++];
                dataMoves++;
            }
            while (i2 < right) {
                temp[k++] = arr[i2++];
                dataMoves++;
            }
            for (int j = left; j < right; j++) {
                arr[j] = temp[j];
                dataMoves++;
            }
        }
    }

    delete[] temp;
}

void printArray(int arr[], int size, const string& label) {
    cout << label << ": ";
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
}

void processAndPrintTopDown(int arr[], int size, const string& label, long long& outTime) {
    resetCounters();
    auto start = high_resolution_clock::now();
    mergeSort(arr, 0, size - 1);
    auto end = high_resolution_clock::now();
    outTime = duration_cast<milliseconds>(end - start).count();

    cout << label << " (Top-Down Merge Sort)\n";
    cout << "First 5 values: ";
    for (int i = 0; i < 5; i++) cout << arr[i] << " ";
    cout << "\nLast 5 values: ";
    for (int i = size - 5; i < size; i++) cout << arr[i] << " ";
    cout << "\nComparisons: " << comparisons << ", Data Moves: " << dataMoves;
    cout << ", Sort Time (ms): " << outTime << "\n\n";
}

void processAndPrintBottomUp(int arr[], int size, const string& label, long long& outTime) {
    resetCounters();
    auto start = high_resolution_clock::now();
    bottomUpMergeSort(arr, size);
    auto end = high_resolution_clock::now();
    outTime = duration_cast<milliseconds>(end - start).count();

    cout << label << " (Bottom-Up Merge Sort)\n";
    cout << "First 5 values: ";
    for (int i = 0; i < 5; i++) cout << arr[i] << " ";
    cout << "\nLast 5 values: ";
    for (int i = size - 5; i < size; i++) cout << arr[i] << " ";
    cout << "\nComparisons: " << comparisons << ", Data Moves: " << dataMoves;
    cout << ", Sort Time (ms): " << outTime << "\n\n";
}

int main() {
    // Original small list test
    int smallArr[] = {4, 3, 1, 0, 9, 8, 6, 7, 5, 2};
    int smallSize = 10;

    printArray(smallArr, smallSize, "Original List");

    resetCounters();
    mergeSort(smallArr, 0, smallSize - 1);
    printArray(smallArr, smallSize, "Sorted List");
    cout << "Comparisons: " << comparisons << ", Data Moves: " << dataMoves << "\n\n";

    // Load large100k.txt
    ifstream file("large100k.txt");
    if (!file) {
        cerr << "Error: large100k.txt not found.\n";
        return 1;
    }

    int* arrTop = new int[100000];
    int* arrBottom = new int[100000];

    for (int i = 0; i < 100000; i++) {
        file >> arrTop[i];
        arrBottom[i] = arrTop[i];
    }

    long long timeTop, timeBottom;
    processAndPrintTopDown(arrTop, 100000, "large100k.txt", timeTop);
    processAndPrintBottomUp(arrBottom, 100000, "large100k.txt", timeBottom);

    if (timeBottom > 0) {
        double ratio = static_cast<double>(timeTop) / timeBottom;
        cout << "Ratio (Top-Down / Bottom-Up): " << ratio << "\n";
    } else {
        cout << "Bottom-Up sort time is 0 ms. Cannot compute ratio.\n";
    }

    delete[] arrTop;
    delete[] arrBottom;
    return 0;
}
