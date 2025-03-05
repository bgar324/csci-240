//notes
  // excercise 1: 
  // brute force and use a nested loop
  // excercise 2: assume no leap year & each year have 365 days
  // for n = 5 to 100, incrementing of 5
  // for e = 1 to 100, run each experiment 100 times
  // ^ within this one, generate 1 birthday & store
  // can also call the function created from excercise 1 -> if return 0, then no one shares the same birthday.
  // but if it returns !0, then people share birthdays


#include <iostream>
using namespace std;

  // so what we can do is initialize a count variable
  // then we iterate through the entire array with a double for loop.
  // in the outer loop, we go from 0 to n-1, incrementing by 1, and check if there is any duplicates by the index
  // in the inner loop, we go from i to n-1, and compare with the index
  // if the two are equal, we increment the counter by 1, if there is another, we increment by 1

int countPair(int arr[], int n){
  int count = 0;
  int holder;
  for(int i = 0; i < n-1; i++){
    holder = arr[i];
    for (int j = i+1; j < n; j++){
      if(holder == arr[j]){
        count++;
      }
    }
  }
  return count;
};

int main() {
  int returnVal;

  int testCase1[5] = {5,7,2,6,4};

  returnVal = countPair(testCase1, 5);

  cout << returnVal << endl;

  int testCase2[8] = {5,7,2,2,5,6,4,2};

  returnVal = countPair(testCase2, 8);

  cout << returnVal << endl;  
};