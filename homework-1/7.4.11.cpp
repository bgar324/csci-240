#include <iostream>
#include <deque>
using namespace std;

int main(){

  deque<int> D;
  deque<int> Q;

  while (!D.empty()) {
    Q.push_back(D.front());  
    D.pop_front();     
  }
}