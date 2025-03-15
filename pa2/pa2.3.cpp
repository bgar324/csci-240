/*
Provide an array implementation to maintain a list of names. The following
operations are available: insert rear, insert at a certain index, remove rear, remove an
element at a certain index, and print the whole list. DO NOT implement an ADT for this
assignment (i.e., do not use a class with data and operations). Just set up a fixed size
array of 10 elements (will not insert more than 10 elements) and provide code in main
calling functions/static methods to perform insert, remove, and print. You can set up a
menu or hard code some test cases to test your implementation, but make sure to run the
test cases below. Feel free to use the provided code from textbook as needed.
*/

/*
  functions
  insertRear: takes a string parameter, then adds it at the last available index
  insertIndex: takes a string and int parameter, then places value at an index, shifting other values
  removeRear: takes no parameters, and removes value at last index
  removeIndex: takes a int paramater, and removes value at index, shifting other avlues
  printList: takes no parameter, and loops through the entire list
*/

#include <iostream>
#include <string>
using namespace std;

//making sure its publicly available
string arr[10];
int numElems = 0;

bool checkSize(){
  if(numElems >= 10){
    return true;
  }
  return false;
};

void insertRear(string name){
  if(checkSize()){ //edge case but kind of not
    cout << "array is full." << endl;
    return;
  }
  //since numElems is indirectly rear, then we can just do a simple index to set it equal to name
  arr[numElems] = name;
  numElems++;
};

void insertIndex(string name, int index){
  if(checkSize()){ //edge case but kind of not
    cout << "array is full." << endl;
    return;
  };

  //need 2 also check for incorrect index param
  if(index < 0 || index >= 10){
    cout << "bad index" << endl;
    return;
  };


  // so we have to do arr[index] = name yes. but how will this effect other elements?
  //it will move everything else one right. 
  //hold on how will this effect numElems... hmphhh.. uhh, i guess checksize wouldnt matter here
  // or would it? prompt says to not insert more than 10, which eliminates edge cases where we would remove rear to make space?
  // ok i think my intution is wrong; just reject it

  //ok so hm. i want to do the insertion last so i can avoid losing data so i will loop at
  for(int i = numElems; i > index; i--){
    arr[i] = arr[i-1];
  }
  arr[index] = name;
  numElems++; 
};

void removeRear(){
  if(numElems == 0){
    cout << "array is empty" << endl;
    return;
  }
  numElems--;
  arr[numElems] = "";
}

void removeIndex(int index){
  //since this will always result in numElems reducing, checking for a full array is counterintuitive
  if(index < 0 || index >= numElems){
    cout << "bad index" << endl;
    return;
  };
  /*
  arr = ["Jo", "Jane", "John", "Kim", "", "", "", "", "", ""];
  lets say remove index 1, num elems = 4
  */
  for(int i = index; i < numElems-1; i++){
    arr[i] = arr[i+1];
  }
  numElems--;
}

void printList(){
  if(numElems == 0){
    cout << "list is empty" << endl;
    return;
  }

  for(int i = 0; i < numElems; i++){
    cout << arr[i];
    if(i < numElems - 1){
      cout << ", "; 
    } 
  }
  cout << endl;
}

int main(){
  cout << "=== Insert Rear ===" << endl;
  insertRear("Jo");
  insertRear("Jane");
  insertRear("John");
  insertRear("Kim");
  printList(); // expected: Jo, Jane, John, Kim

  cout << "\n=== Insert at Index ===" << endl;
  insertIndex("Jake", 1);
  printList(); // expected: Jo, Jake, Jane, John, Kim

  cout << "\n=== Remove at Index ===" << endl;
  removeIndex(2);
  printList(); // expected: Jo, Jake, John, Kim

  cout << "\n=== Remove Rear ===" << endl;
  removeRear();
  printList(); // expected: Jo, Jake, John

  cout << "\n=== Edge Cases ===" << endl;
  removeRear(); removeRear(); removeRear(); // remove everything
  removeRear(); // should print "array is empty."
  printList(); // should print "list is empty."

  cout << "\n=== Insert to Full ===" << endl;
  for(int i = 0; i < 10; i++) {
    insertRear("Name" + to_string(i));
  }
  insertRear("Extra"); // should print "array is full."
  printList(); //should print 10 elements

  cout << "\n=== Invalid Insert Index ===" << endl;
  insertIndex("Invalid", 11); // should print "bad index"

  cout << "\n=== Invalid Remove Index ===" << endl;
  removeIndex(10); // should print "bad index"

  return 0;
  
  return 0;
}