#include <iostream>
#include <vector>
#include <string>

using namespace std;

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

using Entry = std::pair<int, std::string>;

struct MinComparator {
    bool operator()(const Entry &a, const Entry &b) const {
        return a.first < b.first;
    }
};

struct MaxComparator {
    bool operator()(const Entry &a, const Entry &b) const {
        return a.first > b.first;
    }
};

template <typename HEAP>
void testPQ(HEAP& heap, const string& label) {
    cout << "--- Testing " << label << " ---\n";
    heap.insert({5, "five"});
    heap.insert({4, "four"});
    heap.insert({7, "seven"});
    heap.insert({1, "one"});

    cout << "min: " << heap.min().first << " " << heap.min().second << endl;
    heap.remove_min();

    heap.insert({3, "three"});
    heap.insert({6, "six"});

    cout << "min: " << heap.min().first << " " << heap.min().second << endl;
    heap.remove_min();

    cout << "min: " << heap.min().first << " " << heap.min().second << endl;
    heap.remove_min();

    heap.insert({8, "eight"});

    cout << "min: " << heap.min().first << " " << heap.min().second << endl;
    heap.remove_min();

    heap.insert({2, "two"});

    cout << "min: " << heap.min().first << " " << heap.min().second << endl;
    heap.remove_min();

    cout << "min: " << heap.min().first << " " << heap.min().second << endl;
    heap.remove_min();
}

int main() {

  HeapPriorityQueue<Entry, MinComparator> hpMin;
  HeapPriorityQueue<Entry, MaxComparator> hpMax; 

  testPQ(hpMin, "MIN priority (low key wins)");
  testPQ(hpMax, "MAX priority (high key wins)");


  return 0;
}