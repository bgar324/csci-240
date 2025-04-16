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

void sortFileWithPQ(const string& filename, const string& label) {
  ifstream infile(filename);
  if (!infile) {
      cerr << "Error opening file: " << filename << endl;
      return;
  }

  SortedPriorityQueue<Entry, MaxComparator> pq;

  auto start = high_resolution_clock::now();

  int number;
  while (infile >> number) {
      Entry e = {number, to_string(number)};
      pq.insert(e);
  }

  vector<int> sortedValues;
  while (!pq.empty()) {
      sortedValues.push_back(pq.min().first);
      pq.remove_min();
  }

  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - start);

  cout << "--- Sorted result for " << label << " ---" << endl;
  cout << "First 5: ";
  for (int i = 0; i < 5; ++i)
      cout << sortedValues[i] << " ";
  cout << endl;

  cout << "Last 5: ";
  for (int i = sortedValues.size() - 5; i < sortedValues.size(); ++i)
      cout << sortedValues[i] << " ";
  cout << endl;

  cout << "Runtime: " << duration.count() << " ms\n" << endl;
}


int main()
{
  sortFileWithPQ("small1k.txt", "one thousand");
  sortFileWithPQ("large100k.txt", "one hundred thousand");


  return 0;
}