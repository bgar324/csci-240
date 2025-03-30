/*
  Exercise 3 (more points for this exercise): Set up TextEditor class, which can be used
  to store, edit, and display a string of characters using a list implementation. The editor
  (TextEditor class) must utilize either C++ DoublyLinkedList or Java
  LinkedPositionalList where each node stores only 1 character (use composition) and it
  must support the following operations:

  • left() – move cursor left one character (do nothing if at beginning).
  • right() – move cursor right one character (do nothing if at the end).
  • insert(c) – insert the character c just before the cursor.
  • remove() – remove the character just before the cursor (like backspace if
  there is at least one character before it).
  • first() – move cursor to the beginning.
  • last(i) – move cursor to the end.
  • display() – display string (including spaces and cursor).

  Use character | for the cursor so if your characters are "Hello World" and the cursor
  at the beginning then "|Hello World" (i.e, the cursor is before character ‘H’ in the
  example). The cursor shall not be stored in the list but should be printed on the
  screen to indicate where the cursor is at that moment. Note: most operations will be
  based on the current position of the cursor which is just before the iterator and the
  first 7 operations would run in O(1). We will only allow letters in the alphabet and
  space as valid characters. When an editor object is created, it can be an empty string
  as a default option, or you can specify a string such as "Hello World" (assume that
  the cursor will be at the end of the string).
  Set up a driver to create a TextEditor object, then use it to perform a few operations and
  then print the text. Try the following cases below. Just include the driver and class
  TextEditor in the submission of your source code.

  1. Create a TextEditor object with “hHello Word” (assume cursor at the end)
  2. Display current information (should be “hHello Word|”)
  3. Move cursor right (nothing should happen)
  4. Move cursor left
  5. Insert ‘5’ (ignore invalid character)
  6. Insert ‘l’ (letter l)
  7. Display current information (should be “hHello Worl|d”)
  8. Move cursor to the beginning
  9. Move cursor left (nothing should happen)
  10. Move cursor right
  11. Remove character
  12. Move cursor to the end
  13. Display current information (should be “Hello World|”)
*/


#include <iostream>
#include <string>
using namespace std;

// doubly linked list
class Node{
  public:
    char data;
    Node* prev;
    Node* next;
};

class TextEditor{
  private: 
    Node* head;
    Node* tail;
    Node* cursor;

  /*
    • left() – move cursor left one character (do nothing if at beginning).
    • right() – move cursor right one character (do nothing if at the end).
    • insert(c) – insert the character c just before the cursor.
    • remove() – remove the character just before the cursor (like backspace if
    there is at least one character before it).
    • first() – move cursor to the beginning.
    • last(i) – move cursor to the end.
    • display() – display string (including spaces and cursor).
  */  
  public:
    TextEditor(string initialText =  ""){
      head = nullptr;
      tail = nullptr;
      cursor = nullptr;

      for(char c : initialText){
        if (!isalpha(c) && c != ' ') continue;
        Node* temp = new Node();
        temp->data = c;
        temp->prev = tail;
        temp->next = nullptr;

        if (tail != nullptr) {
          tail->next = temp;
        } else {
          head = temp;
        }
        tail = temp;
      }
      cursor = nullptr;
    };

    void left() {
      if (head == nullptr) return;        
      if (cursor == head) return;     
      
      if (cursor == nullptr) {
          cursor = tail;
      } else {
          cursor = cursor->prev;
      }
  }
  

  void right() {
    if (head == nullptr) return;  
    if (cursor == nullptr) return;    

    if (cursor == tail) {
        cursor = nullptr;
    } else {
        cursor = cursor->next;
    }
}


    void insert(char c){
      if (!isalpha(c) && c != ' ') return;
  
      Node* temp = new Node();
      temp->data = c;
      temp->next = cursor;
  
      if (cursor == nullptr) {
          temp->prev = tail;
          if (tail != nullptr) tail->next = temp;
          else head = temp;
          tail = temp;
      } else {
          temp->prev = cursor->prev;
          if (cursor->prev != nullptr) {
              cursor->prev->next = temp;
          } else {
              head = temp;
          }
          cursor->prev = temp;
      }
  
      cursor = temp->next;
  }
  
  

    void remove(){
      if(head == nullptr || cursor == head){
        return; // nothing to remove
      }

      Node* toDelete = cursor ? cursor->prev : tail;

      if(toDelete == nullptr){
        return;
      }
      if (toDelete->prev != nullptr) {
        toDelete->prev->next = cursor;
      } else {
        head = cursor; // we're removing the head
      }
    
      if (cursor != nullptr) {
        cursor->prev = toDelete->prev;
      } else {
        tail = toDelete->prev; // we’re removing the tail
      }
    
      delete toDelete;
    };

    void first(){
      cursor = head;
    };

    void last(){
      cursor = nullptr;
    };

    void display(){
      Node* it = head;
      while(it != nullptr){
        if(it == cursor){
          cout << "|";  
        }
        cout << it->data;
        it = it->next;
      }
      if(cursor == nullptr){
        cout << "|";
      }
      cout << endl;
    };
};  

int main() {
  TextEditor editor("hHello Word");

  int option;
  char inputChar;

  do {
    cout << "\n--- Text Editor Menu ---\n";
    cout << "1. Move cursor left\n";
    cout << "2. Move cursor right\n";
    cout << "3. Insert character\n";
    cout << "4. Remove character\n";
    cout << "5. Move cursor to beginning\n";
    cout << "6. Move cursor to end\n";
    cout << "7. Display current text\n";
    cout << "8. Quit\n";
    cout << "Choose an option: ";
    cin >> option;

    switch(option) {
      case 1:
        editor.left();
        cout << "Moved cursor left.\n";
        break;
      case 2:
        editor.right();
        cout << "Moved cursor right.\n";
        break;
      case 3:
        cout << "Enter character to insert: ";
        cin >> inputChar;
        if (isalpha(inputChar) || inputChar == ' ') {
          editor.insert(inputChar);
          cout << "Character '" << inputChar << "' inserted.\n";
        } else {
          cout << "Invalid character. Only letters and spaces are allowed.\n";
        }
        break;
      case 4:
        editor.remove();
        cout << "Removed character before cursor.\n";
        break;
      case 5:
        editor.first();
        cout << "Cursor moved to beginning.\n";
        break;
      case 6:
        editor.last();
        cout << "Cursor moved to end.\n";
        break;
      case 7:
        cout << "Current text: ";
        editor.display();
        break;
      case 8:
        cout << "Exiting editor.\n";
        break;
      default:
        cout << "Invalid option. Try again.\n";
    }

  } while(option != 8);

  return 0;
}
