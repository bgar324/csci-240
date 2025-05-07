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

int medianOfThree(int arr[], int left, int right) {
    int mid = left + (right - left) / 2;
    if (++comparisons && arr[left] > arr[mid]) swap(arr[left], arr[mid]), dataMoves += 2;
    if (++comparisons && arr[left] > arr[right]) swap(arr[left], arr[right]), dataMoves += 2;
    if (++comparisons && arr[mid] > arr[right]) swap(arr[mid], arr[right]), dataMoves += 2;
    return mid;
}

int partition(int arr[], int left, int right) {
    int pivotIndex = medianOfThree(arr, left, right);
    int pivot = arr[pivotIndex];
    dataMoves++;
    swap(arr[pivotIndex], arr[right]);
    dataMoves += 2;

    int i = left - 1;
    for (int j = left; j < right; j++) {
        comparisons++;
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
            dataMoves += 2;
        }
    }
    swap(arr[i + 1], arr[right]);
    dataMoves += 2;
    return i + 1;
}

void quickSort(int arr[], int left, int right) {
    if (left < right) {
        int pi = partition(arr, left, right);
        quickSort(arr, left, pi - 1);
        quickSort(arr, pi + 1, right);
    }
}

void printArray(int arr[], int size, const string& label) {
    cout << label << ": ";
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
}

void processAndPrint(int arr[], int size, const string& label) {
    resetCounters();
    auto start = high_resolution_clock::now();
    quickSort(arr, 0, size - 1);
    auto end = high_resolution_clock::now();
    auto sortTime = duration_cast<milliseconds>(end - start).count();

    cout << label << endl;
    cout << "First 5 values: ";
    for (int i = 0; i < 5; i++) cout << arr[i] << " ";
    cout << "\nLast 5 values: ";
    for (int i = size - 5; i < size; i++) cout << arr[i] << " ";
    cout << "\nComparisons: " << comparisons << ", Data Moves: " << dataMoves;
    cout << ", Sort Time (ms): " << sortTime << "\n\n";
}

int main() {
    int smallArr[] = {4, 3, 1, 0, 9, 8, 6, 7, 5, 2};
    int smallSize = 10;

    printArray(smallArr, smallSize, "Original List");

    resetCounters();
    quickSort(smallArr, 0, smallSize - 1);
    printArray(smallArr, smallSize, "Sorted List");
    cout << "Comparisons: " << comparisons << ", Data Moves: " << dataMoves << "\n\n";

    ifstream file1("small1k.txt");
    int* bigArr1 = new int[1000];
    for (int i = 0; i < 1000; i++) file1 >> bigArr1[i];
    processAndPrint(bigArr1, 1000, "Data from small1k.txt");
    delete[] bigArr1;

    ifstream file2("large100k.txt");
    int* bigArr2 = new int[100000];
    for (int i = 0; i < 100000; i++) file2 >> bigArr2[i];
    processAndPrint(bigArr2, 100000, "Data from large100k.txt");
    delete[] bigArr2;

    return 0;
}
