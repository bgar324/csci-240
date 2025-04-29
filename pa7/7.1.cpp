#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
using namespace std;

string generateStringFromKey(int n){
  string reversed = "";
  if (n == 0){
    return "0";
  }

  while (n > 0){
    int digit = n % 10;
    reversed += to_string(digit);
    n /= 10;
  }

  return reversed;
}

void found(int n, string k){
  cout << "key: " << n << endl;
  cout << "value: " << k << endl;
}

int main() {
  unordered_map<int, string> myMap;
  vector<int> vec = {13, 21, 5, 37, 15};
  for(int i = 0; i < vec.size(); i++){
    myMap[vec.at(i)] = generateStringFromKey(vec.at(i));
  } // generates the unordered map with the key & string

  // deleting / finding

  for(auto it = myMap.begin(); it != myMap.end(); ){
    if(it->first == 10 || it->first == 21){
      found(it->first, it->second);
      ++it;
    } else if (it->first == 20 || it->first == 37){
      it = myMap.erase(it);
    } else {
      ++it;
    }
  }

  cout << endl;

  cout << "--- checking hash map again after changes ---" << endl;

  cout << endl;

  for(auto it = myMap.begin(); it != myMap.end(); ++it){
    found(it->first, it->second);
  }

  cout << endl;

  cout << "--- small 1k portion ---" << endl;

  ifstream file("small1k.txt");
  vector<int> small1kdata;
  int val;
  while(file >> val){
    small1kdata.push_back(val);
  }
  file.close();

  unordered_map<int, string> small1kMap;
  small1kMap.reserve(1000 / 0.75);

  auto start = chrono::high_resolution_clock::now();

  for (int value : small1kdata) {
    small1kMap[value] = generateStringFromKey(value);
  }

  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed = end - start;
  cout << "Time to insert 1000 entries: " << elapsed.count() << " seconds" << endl;

  cout << endl;

  cout << "--- large 100k portion ---" << endl;

  ifstream file2("large100k.txt");
  vector<int> large100kdata;
  int val2;
  while(file2 >> val2){
    large100kdata.push_back(val2);
  }
  file2.close();

  unordered_map<int, string> large100kMap;
  large100kMap.reserve(100000 / 0.75);

  auto start2 = chrono::high_resolution_clock::now();

  for (int value : large100kdata) {
    large100kMap[value] = generateStringFromKey(value);
  }

  auto end2 = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed2 = end2 - start2;
  cout << "Time to insert 100,000 entries: " << elapsed2.count() << " seconds" << endl;

  return 0;
}