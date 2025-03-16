/*
  Provide a linked list implementation using only the head of the list and
  numElems to maintain a list of names (do not use a variable to refer to the tail of the list).
  The following operations are available: insert rear, insert front, remove a particular
  element, and print the whole list. Do not implement an ADT for this lab (i.e., do not use
  a class with data and operations). Just set up a node and use head pointer/reference to
  maintain a linked list of nodes and provide code in main calling functions/static methods
  to perform insert, remove, and print. You can set up a menu or hard code some test cases
  to test your implementation, but make sure to run the test cases below. Feel free to use
  the provided code from textbook as needed
*/

#include <iostream>
#include <string>
using namespace std;

struct Node{
  string name; 
  Node* next;
};


Node* head = nullptr;
int numElems;

//idk man this probably works probably..
void insertFront(string name){
  // ok um, so insert front, so basically put something as the head
  Node* temp = new Node();
  // ^ create temp to set stuff
  temp->next = head;
  temp->name = name;
  head = temp;
  numElems++;
};

void insertRear(string name){
  //idk if this works..
  if(head == nullptr){
    head = new Node();
    head->name = name;
    head->next = nullptr;
    numElems++;
    return;
  }

  //ok so temp is equal head's struct
  Node* temp = head;
  while(temp->next != nullptr){ //!= so that we can keep searching until temp is the last linked
    temp = temp->next; //temp is not equal to the next linked value
  }

  //once temp is equal to the last value
  //create a newNode to attach to the previous tail
  Node* newNode = new Node();
  //setting variables
  newNode->name = name;
  //setting newNode's next to be a nullptr, an attribute of a tail.
  newNode->next = nullptr;
  //temmp's next is now connected to this tail.
  temp->next = newNode;
  numElems++;
};

void removeElem(string name){
  if(head == nullptr){
    cout << "list is empty." << endl;
    return;
  }

  /*
    branches of thought
    head is the only thing in the linked list, and it is the name
    head is the only thing in the linked list, but the name isnt there.
  
  */
  

  if(head->name == name && head->next == nullptr){ //ok so if the first node (head)'s name equates to name , then we should basically increment on the linked list, then set the head to be the temp. 
    Node* temp = head;
    head = head->next;
    delete temp;
    return;
  };


  Node* prev = head;
  Node* temp = head->next;
  while(temp != nullptr){
    if(temp->name == name){
      prev->next = temp->next;
      delete temp;
      return;
    }
    prev = temp;
    temp = temp->next;
  }

  cout << "name not here." << endl;
  return;
}

void printList() {
  Node* temp = head;
  while (temp != nullptr) {
      cout << temp->name;
      if (temp->next != nullptr) cout << " → ";
      temp = temp->next;
  }
  cout << endl;
}

int main() {
  // Insert at front
  insertFront("Jo");
  insertFront("Jane");
  insertFront("John");
  
  cout << "List after insertFront calls: ";
  printList();  // Expected: John → Jane → Jo

  // Insert at rear
  insertRear("Kim");
  insertRear("Anna");

  cout << "List after insertRear calls: ";
  printList();  // Expected: John → Jane → Jo → Kim → Anna

  // Remove an element that exists
  removeElem("Jo");
  cout << "List after removing 'Jo': ";
  printList();  // Expected: John → Jane → Kim → Anna

  // Remove first element (head)
  removeElem("John");
  cout << "List after removing 'John' (head): ";
  printList();  // Expected: Jane → Kim → Anna

  // Remove last element (tail)
  removeElem("Anna");
  cout << "List after removing 'Anna' (tail): ";
  printList();  // Expected: Jane → Kim

  // Try to remove an element that doesn’t exist
  removeElem("Tom");
  cout << "List after trying to remove 'Tom' (not in list): ";
  printList();  // Expected: Jane → Kim

  return 0;
}