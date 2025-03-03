#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

// we need to generate an array of i length and produced j times

using namespace std;

int countPair(vector<int> arr, int n){
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

vector<int> createBirthdays(int length){
  vector<int> birthdays(length);
  for(int i = 0; i < length; i++){
    birthdays[i] = (rand()%365) + 1;
  }
  return birthdays;
};

int main(){
  int count;
  int chances;
  int percentageChance;
  srand((unsigned)time(0));

  // ok so this gives us a value 
  for(int i = 5; i <= 100; i+=5){
    count = 0;
    for(int j = 0; j < 100; j ++){
      percentageChance = countPair(createBirthdays(i), i);
      if (percentageChance > 0){
        count++;
      }
    }
    double chances = (static_cast<double>(count) / 100.0) * 100.0;
    cout << i << " People: " << chances << "%" << endl;
  }

  return 0;
}

/*
run experiment 100 times, why? to make a 
*/