#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

template <typename T>
vector<T> shuffleVector(vector<T> vec){
  random_device rd;
  mt19937 g(rd());

  shuffle(vec.begin(), vec.end(), g);

  return vec;
}

int main(){
  vector<int> test = {1,2,3,4,5};

  vector<int> shuffled = shuffleVector(test);

  for(int val : shuffled){
    cout << val << ", ";
  }

  return 0;

}