#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

template <typename Entry, typename Compare=less<Entry>>
class HeapPriorityQueue {
  protected:
    typename std::vector<Entry> data;                           // our underlying collection
    Compare less_than;                                     // less_than(a,b) defines "a < b" relationship

    int parent(int j) { return (j-1) / 2; }                // truncating division
    int left(int j) { return 2 * j + 1; }
    int right(int j) { return 2 * j + 2; }
    bool has_left(int j) { return left(j) < data.size(); }
    bool has_right(int j) { return right(j) < data.size(); }
    
    // Moves the entry at index j higher, if necessary, to restore the heap property
    void upheap(int j) {
        while (j > 0) {                                    // continue to root (or break statement)
            int p{parent(j)};
            if (less_than(data[j], data[p])) {             // if j's entry is less than its parent
                swap(data[j], data[p]);
                j = p;                                     // continue from new location after the swap
            } else
                break;                                     // j is properly placed relative to parent
        }
    }
    
    // Moves the entry at index j lower, if necessary, to restore the heap property
    void downheap(int j) {
        while (has_left(j)) {                              // continue to bottom (or break statement)
            int small_child{left(j)};                      // although right may be smaller
            if (has_right(j)) {
                if (less_than(data[right(j)], data[left(j)]))
                    small_child = right(j);
            }

            if (less_than(data[small_child], data[j])) {   // if small child's entry is less than j's
                swap(data[j], data[small_child]);
                j = small_child;                           // continue from new location after the swap
            } else
                break;
        }
    }

  public:
    HeapPriorityQueue() {}                                 // default is an empty priority queue

    int size() const { return data.size(); }               // returns number of entries
    bool empty() const { return data.empty(); }            // returns true if zero entries
    const Entry& min() const { return data.front(); }      // returns reference to root entry

    // Inserts a new entry into the priority queue
    void insert(const Entry& e) {
        data.push_back(e);                                 // add to the end of the vector
        upheap(data.size() - 1);                           // upheap newly added entry
    }
    
    // Removes the minimum entry
    void remove_min() {
        swap(data[0], data[data.size() - 1]);              // put last entry at the root
        data.pop_back();                                   // and remove the displaced minimum
        downheap(0);                                       // fix heap property for new root
    }
};

using Entry = std::pair<string, int>; // Changed to string, int for string comparison

struct StringMaxComparator {
    bool operator()(const Entry &a, const Entry &b) const {
        return a.first > b.first; // Will sort strings in descending order
    }
};

void sortFileWithHeap(const string& filename, const string& label) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    HeapPriorityQueue<Entry, StringMaxComparator> pq;

    auto start = high_resolution_clock::now();

    int number;
    while (infile >> number) {
        Entry e = {to_string(number), number};
        pq.insert(e);
    }

    ofstream outfile(label + "_sorted.txt");
    if (!outfile) {
        cerr << "Error creating output file: " << label + "_sorted.txt" << endl;
        return;
    }

    vector<string> sortedValues;
    while (!pq.empty()) {
        Entry e = pq.min();
        sortedValues.push_back(e.first);

        if (sortedValues.size() % 5 == 0) {
            for (int i = sortedValues.size() - 5; i < sortedValues.size(); ++i) {
                outfile << sortedValues[i] << " ";
            }
            outfile << endl;
        }
        
        pq.remove_min();
    }

    int remainingCount = sortedValues.size() % 5;
    if (remainingCount > 0) {
        for (int i = sortedValues.size() - remainingCount; i < sortedValues.size(); ++i) {
            outfile << sortedValues[i] << " ";
        }
        outfile << endl;
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "--- Sorted result for " << label << " ---" << endl;
    cout << "First 5: ";
    for (int i = 0; i < min(5, (int)sortedValues.size()); ++i)
        cout << sortedValues[i] << " ";
    cout << endl;

    cout << "Last 5: ";
    for (int i = max(0, (int)sortedValues.size() - 5); i < sortedValues.size(); ++i)
        cout << sortedValues[i] << " ";
    cout << endl;

    cout << "Runtime: " << duration.count() << " ms\n" << endl;
    
    outfile.close();
}

int main() {
    sortFileWithHeap("small1k.txt", "small1k");
    sortFileWithHeap("large100k.txt", "large100k");

    return 0;
}