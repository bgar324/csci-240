#include <iostream>
using namespace std;

int sum_odd_to_n(int n){
  int count = 0;
  for(int i = 1; i <= n; i += 2){
    count += i;
  }
  return count;
}

int main(){

  cout << sum_odd_to_n(8) << endl;

  return 0;
}