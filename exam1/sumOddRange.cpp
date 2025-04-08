#include <iostream>
using namespace std;

int sumOddRange(int a , int b){
  int sum = 0;
  for(int i = a; i <= b; i++){
    if(i % 2 == 1){
      sum += i;
    }
  }

  return sum;

}

int main(){

  int result = sumOddRange(3,8);
  cout << result << endl;

  return 0;
}