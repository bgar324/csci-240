#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


//take an array from 1 to 52 
//then shuffle them?

void shuffleDeck(int deck[], int size) {
  std::srand(std::time(0)); 
  for (int i = size - 1; i > 0; --i) {
      int j = std::rand() % (i + 1);
      std::swap(deck[i], deck[j]);  
  }
}

int main(){

  int deck[52];

  for(int i = 0; i < 52; ++i){
    deck[i] = i+1;
  }

  shuffleDeck(deck, 52);

  for(int i = 0; i < 52; i++){
    if(i == 51){
      cout << deck[i] << endl;
    }
    cout << deck[i] << ", ";
  }

  return 0;
}