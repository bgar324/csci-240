#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <stdexcept>
using namespace std;

// merge sort helpers
void mergeSortMerge(vector<int>& arr, int left, int mid, int right, vector<int>& temp) {
    int i = left, j = mid + 1, k = left;
    while(i <= mid && j <= right) {
        if(arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while(i <= mid) temp[k++] = arr[i++];
    while(j <= right) temp[k++] = arr[j++];
    for(int idx = left; idx <= right; ++idx) arr[idx] = temp[idx];
}

void mergeSortRec(vector<int>& arr, int left, int right, vector<int>& temp) {
    if(left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortRec(arr, left, mid, temp);
    mergeSortRec(arr, mid + 1, right, temp);
    mergeSortMerge(arr, left, mid, right, temp);
}

void mergeSort(vector<int>& arr) {
    int n = (int)arr.size();
    if(n <= 1) return;
    vector<int> temp(n);
    mergeSortRec(arr, 0, n-1, temp);
}

// process one file and print result
void processFile(const char* filename) {
    ifstream fin(filename);
    if(!fin) {
        cout << "Could not open " << filename << "\n";
        return;
    }
    vector<int> data;
    int x;
    while(fin >> x) data.push_back(x);
    fin.close();
    int n = data.size();

    auto t0 = chrono::high_resolution_clock::now();

    mergeSort(data);

    // uniqueness check
    bool unique = true;
    for(int i = 1; i < n; ++i) {
        if(data[i] == data[i-1]) {
            unique = false;
            break;
        }
    }
    long long pairs = (long long)n * (n-1) / 2;

    auto t1 = chrono::high_resolution_clock::now();
    long long ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();

    cout << n << "  " << (unique ? "yes" : "no") << "  " << pairs << "  " << ms << " ms\n";
}

int main() {
    processFile("infile1.txt");
    processFile("infile2.txt");
    return 0;
}
