#include <iostream>
#include <list>

using namespace std;

template <typename Entry, typename Compare=less<Entry>>
class SortedPriorityQueue {
  protected:
    typename std::list<Entry> data;                            // our underlying collection
    Compare less_than;                                    // less_than(a,b) defines "a < b" relationship

  public:
    SortedPriorityQueue() {}                              // default is an empty priority queue

    int size() const { return data.size(); }              // returns number of entries
    bool empty() const { return data.empty(); }           // returns true if there are no entries
    const Entry& min() const { return data.front(); }     // returns constant reference to minimum entry
    void remove_min() { data.pop_front(); }               // removes the minimum entry

    // Inserts a new entry into the priority queue
    void insert(const Entry& e) {
        typename list<Entry>::iterator walk{data.begin()};
        while (walk != data.end() && less_than(*walk, e)) // while walk's entry is less than new entry
            ++walk;                                       // advance walk
        data.insert(walk, e);                             // new element goes before walk
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

template <typename PQ>
void testPQ(PQ& pq, const string& label) {
    cout << "--- Testing " << label << " ---\n";
    pq.insert({5, "five"});
    pq.insert({4, "four"});
    pq.insert({7, "seven"});
    pq.insert({1, "one"});

    cout << "min: " << pq.min().first << " " << pq.min().second << endl;
    pq.remove_min();

    pq.insert({3, "three"});
    pq.insert({6, "six"});

    cout << "min: " << pq.min().first << " " << pq.min().second << endl;
    pq.remove_min();

    cout << "min: " << pq.min().first << " " << pq.min().second << endl;
    pq.remove_min();

    pq.insert({8, "eight"});

    cout << "min: " << pq.min().first << " " << pq.min().second << endl;
    pq.remove_min();

    pq.insert({2, "two"});

    cout << "min: " << pq.min().first << " " << pq.min().second << endl;
    pq.remove_min();

    cout << "min: " << pq.min().first << " " << pq.min().second << endl;
    pq.remove_min();
}


int main()
{
  SortedPriorityQueue<Entry, MinComparator> pqMin;
  SortedPriorityQueue<Entry, MaxComparator> pqMax; 

  testPQ(pqMin, "MIN priority (low key wins)");
  testPQ(pqMax, "MAX priority (high key wins)");
  
  return 0;
}