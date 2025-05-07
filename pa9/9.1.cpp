#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace chrono;

int comparisons = 0;
int dataMoves = 0;

void insertionSort(int array[], int size) {
    for (int index = 1; index < size; index++) {
        int unsortedValue = array[index];
        dataMoves++;
        int scan = index;
        while (scan > 0 && ++comparisons && array[scan - 1] > unsortedValue) {
            array[scan] = array[scan - 1];
            dataMoves++;
            scan--;
        }
        array[scan] = unsortedValue;
        dataMoves++;
    }
}

int countInversions(int array[], int size) {
    int numInversions = 0;
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (array[i] > array[j]) {
                numInversions++;
            }
        }
    }
    return numInversions;
}

void printArray(int array[], int size) {
    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

void resetCounters() {
    comparisons = 0;
    dataMoves = 0;
}

int main() {
    int smallArr[] = {4, 3, 1, 0, 9, 8, 6, 7, 5, 2};
    int size = 10;

    cout << "Original List: ";
    printArray(smallArr, size);

    int inv = countInversions(smallArr, size);
    cout << "Inversions: " << inv << endl;

    resetCounters();
    insertionSort(smallArr, size);
    cout << "Sorted List: ";
    printArray(smallArr, size);
    cout << "Inversions: " << countInversions(smallArr, size) << endl;
    cout << "Comparisons: " << comparisons << ", Data Moves: " << dataMoves << endl;

    reverse(smallArr, smallArr + size);
    cout << "Reversed List: ";
    printArray(smallArr, size);

    resetCounters();
    insertionSort(smallArr, size);
    cout << "Resorted List: ";
    printArray(smallArr, size);
    cout << "Inversions: " << countInversions(smallArr, size) << endl;
    cout << "Comparisons: " << comparisons << ", Data Moves: " << dataMoves << endl;

    int bigArr[1000];
    ifstream file("small1k.txt");
    for (int i = 0; i < 1000; i++) {
        file >> bigArr[i];
    }

    cout << "\nFirst 5 values of 1000-element list: ";
    for (int i = 0; i < 5; i++) cout << bigArr[i] << " ";
    cout << "\nLast 5 values: ";
    for (int i = 995; i < 1000; i++) cout << bigArr[i] << " ";
    cout << endl;

    int invBig = countInversions(bigArr, 1000);
    cout << "Inversions before sort: " << invBig << endl;

    resetCounters();
    auto start = high_resolution_clock::now();
    insertionSort(bigArr, 1000);
    auto end = high_resolution_clock::now();
    auto sortTime = duration_cast<milliseconds>(end - start).count();

    cout << "Inversions after sort: " << countInversions(bigArr, 1000) << endl;
    cout << "Comparisons: " << comparisons << ", Data Moves: " << dataMoves << endl;
    cout << "Sort Time (ms): " << sortTime << endl;

    start = high_resolution_clock::now();
    countInversions(bigArr, 1000);
    end = high_resolution_clock::now();
    auto inversionTime = duration_cast<milliseconds>(end - start).count();
    cout << "Inversion Count Time (ms): " << inversionTime << endl;

    return 0;
}