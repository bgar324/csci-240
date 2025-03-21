#include <iostream>
#include <string>
using namespace std;

struct Node {
    string val;
    Node* next;
};

void addFront(Node*& head, string value){
  Node* temp = new Node();
  temp->val = value;
  temp->next = head;
  head = temp;
}

void addRear(Node*& tail, string value){
  Node* temp = new Node();
  temp->val = value;
  temp->next = nullptr;
  tail->next = temp;
  tail = temp;
}

void printList(Node* head){
  Node* iterator = head;
  while(iterator != nullptr){
    cout << iterator->val << ", ";
    iterator = iterator->next;
  }
  cout << endl;
}

void insertBefore(Node*& head, Node*& it, string value) {
  if (head == it) {  
      addFront(head, value);
      it = head->next; 
      return;
  }

  Node* prev = head;
  while (prev->next != it) {  
      prev = prev->next;
  }

  Node* temp = new Node();
  temp->val = value;
  temp->next = it;
  prev->next = temp;
}

void removeFront(Node*& head) {
  if (head == nullptr) return;
  Node* temp = head;
  head = head->next;
  delete temp;
}

void removeBack(Node*& head, Node*& tail) {
  if (head == nullptr) return; 

  if (head == tail) {  
      delete head;
      head = nullptr;
      tail = nullptr;
      return;
  }

  Node* temp = head;
  while (temp->next != tail) {
      temp = temp->next;
  }
  
  delete tail;
  tail = temp;
  tail->next = nullptr;
}

int main() {

  Node* head = nullptr;
  Node* tail = nullptr;

  addFront(head, "Two");
  tail = head;

  addRear(tail, "Three");
  addFront(head, "One"); 
  addRear(tail, "Four"); 

  cout << "Current list: " << endl;
  printList(head);

  cout << "Initial list: ";
  printList(head);

  // Step 6: Set up an iterator and advance twice
  Node* it = head;
  if (it != nullptr) it = it->next;
  if (it != nullptr) it = it->next; // Iterator is now at "Three"

  // Step 7: Insert your name before the iterator
  insertBefore(head, it, "Benjamin");

  // Step 8: Remove the front element
  removeFront(head);

  // Step 9: Remove the back element
  removeBack(head, tail);

  // Step 10: Print final list using iterator
  cout << "Final list: ";
  printList(head);

  return 0;
}