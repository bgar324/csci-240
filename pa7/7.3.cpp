#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <vector>

using namespace std;

const int BIG_PRIME = 2147483647; // 2^31 - 1

// Polynomial hash function: hash(s) = s[0]*a^(n-1) + ... + s[n-1]*a^0 mod BIG_PRIME
int polynomial_hash(const string& s, int a) {
    unsigned long long hash = 0;
    for (char c : s) {
        hash = (hash * a + c) % BIG_PRIME;
    }
    return static_cast<int>(hash);
}

int cyclic_shift_hash(const string& s) {
  unsigned int hash = 0;
  for (char c : s) {
      hash = (hash << 5) | (hash >> 27); // rotate left 5 bits
      hash += c;
  }
  return static_cast<int>(hash % BIG_PRIME);
}


int main() {
    ifstream file("USDeclIndFormatted.txt");
    if (!file) {
        cerr << "Failed to open usdeclar.txt" << endl;
        return 1;
    }

    set<string> unique_words;
    string word;
    while (file >> word) {
        unique_words.insert(word);
    }
    file.close();

    cout << "Unique words: " << unique_words.size() << endl << endl;

    vector<int> a_values = {1, 37, 40, 41};

    for (int a : a_values) {
        map<int, vector<string>> hash_map;

        for (const string& w : unique_words) {
            int h = polynomial_hash(w, a);
            hash_map[h].push_back(w);
        }

        int collision_count = 0;
        for (const auto& [hash_val, word_list] : hash_map) {
            if (word_list.size() > 1)
                collision_count++;
        }

        cout << "a = " << a << " → collisions: " << collision_count << endl;
    }

    map<int, vector<string>> cylic_hash_map;
    for (const string& w : unique_words) {
      int h = cyclic_shift_hash(w);
      cylic_hash_map[h].push_back(w);
    }

    int cyclic_collision_count = 0;
    for (const auto& [hash_val, word_list] : cylic_hash_map) {
      if (word_list.size() > 1)
        cyclic_collision_count++;
    }

    cout << "Cyclic shift hash collisions: " << cyclic_collision_count << endl;

    return 0;
}
