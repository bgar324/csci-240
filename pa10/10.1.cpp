#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>

using namespace std;

class QuickSelect {
private:
    int compares;
    int recursiveCalls;

public:
    QuickSelect() : compares(0), recursiveCalls(0) {}

    // Reset counters
    void reset() {
        compares = 0;
        recursiveCalls = 0;
    }

    // Partition function similar to quicksort
    int partition(vector<int>& arr, int left, int right) {
        // Choose a random pivot
        int pivotIndex = left + rand() % (right - left + 1);
        int pivotValue = arr[pivotIndex];
        
        // Move pivot to the end
        swap(arr[pivotIndex], arr[right]);
        
        // Partition the array
        int storeIndex = left;
        for (int i = left; i < right; i++) {
            compares++;
            if (arr[i] < pivotValue) {
                swap(arr[i], arr[storeIndex]);
                storeIndex++;
            }
        }
        
        // Move pivot to its final place
        swap(arr[storeIndex], arr[right]);
        return storeIndex;
    }

    // Main quick select function
    int select(vector<int>& arr, int left, int right, int k) {
        if (left == right) {
            return arr[left];
        }
        
        recursiveCalls++;
        
        // Partition the array and get the position of the pivot
        int pivotIndex = partition(arr, left, right);
        
        // The pivot is in its final sorted position
        if (k == pivotIndex) {
            return arr[k];
        } 
        // If k is less than the pivot position, search in the left subarray
        else if (k < pivotIndex) {
            return select(arr, left, pivotIndex - 1, k);
        } 
        // If k is greater than the pivot position, search in the right subarray
        else {
            return select(arr, pivotIndex + 1, right, k);
        }
    }

    // Wrapper function
    int findKthSmallest(vector<int>& arr, int k) {
        reset();
        if (k < 0 || k >= arr.size()) {
            throw out_of_range("k is out of range");
        }
        return select(arr, 0, arr.size() - 1, k);
    }

    int getCompares() const {
        return compares;
    }

    int getRecursiveCalls() const {
        return recursiveCalls;
    }
};

// Function to read numbers from a file
vector<int> readFromFile(const string& filename) {
    vector<int> numbers;
    ifstream file(filename);
    int num;
    
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        return numbers;
    }
    
    while (file >> num) {
        numbers.push_back(num);
    }
    
    file.close();
    return numbers;
}

int main() {
    srand(time(nullptr));
    QuickSelect qs;
    
    // Test with the given list
    vector<int> arr = {4, 3, 1, 0, 9, 8, 6, 7, 5, 2};
    int n = arr.size();
    
    // Test k = 1 (smallest)
    vector<int> arr1 = arr;
    int k1 = 0; // 0-indexed, so k=1 is index 0
    int result1 = qs.findKthSmallest(arr1, k1);
    cout << "k = 1 (smallest): " << result1 << endl;
    cout << "Number of compares: " << qs.getCompares() << endl;
    cout << "Number of recursive calls: " << qs.getRecursiveCalls() << endl;
    cout << endl;
    
    // Test k = n/2 (median)
    vector<int> arr2 = arr;
    int k2 = n/2;
    int result2 = qs.findKthSmallest(arr2, k2);
    cout << "k = n/2 (median): " << result2 << endl;
    cout << "Number of compares: " << qs.getCompares() << endl;
    cout << "Number of recursive calls: " << qs.getRecursiveCalls() << endl;
    cout << endl;
    
    // Test k = n (largest)
    vector<int> arr3 = arr;
    int k3 = n-1; // 0-indexed, so k=n is index n-1
    int result3 = qs.findKthSmallest(arr3, k3);
    cout << "k = n (largest): " << result3 << endl;
    cout << "Number of compares: " << qs.getCompares() << endl;
    cout << "Number of recursive calls: " << qs.getRecursiveCalls() << endl;
    cout << endl;
    
    // Test with small1k.txt
    vector<int> small1k = readFromFile("small1k.txt");
    if (!small1k.empty()) {
        int median_small = qs.findKthSmallest(small1k, small1k.size()/2);
        cout << "Median of small1k.txt: " << median_small << endl;
        cout << "Number of compares: " << qs.getCompares() << endl;
        cout << "Number of recursive calls: " << qs.getRecursiveCalls() << endl;
        cout << endl;
    }
    
    // Test with large100k.txt
    vector<int> large100k = readFromFile("large100k.txt");
    if (!large100k.empty()) {
        int median_large = qs.findKthSmallest(large100k, large100k.size()/2);
        cout << "Median of large100k.txt: " << median_large << endl;
        cout << "Number of compares: " << qs.getCompares() << endl;
        cout << "Number of recursive calls: " << qs.getRecursiveCalls() << endl;
    }
    
    return 0;
}