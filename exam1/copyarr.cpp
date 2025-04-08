#include <iostream>
using namespace std;

void changeAndCopyArrSize(int*& arr, int prevSize, int newSize){
  int* newArray = new int[newSize-1];
  for(int i = 0; i < prevSize-1; i++){
    newArray[i] = arr[i];
  }
  delete[] arr;
  arr = newArray;
}

int main() {


  return 0;
}