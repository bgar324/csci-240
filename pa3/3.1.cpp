#include <iostream>
#include <vector>
#include <string>

using namespace std;

class myArrayVector{
  public:
    vector<string> vec;
    myArrayVector(vector<string> x){
      vec = x;
    }
    
    // are we removing the element or index, im i think its the index, but heres the elem removal
    //overloading function
    void removeElem(string e){
      auto it = vec.begin();
      while(it != vec.end()){
        if(*it == e){
          it = vec.erase(it);
        } else {
          ++it;
        }
      }
    }

    void removeElem(int e){
      // if request: remove element at index e then..:
      if(e < 0 || e > vec.size()){
        cout << "bad index" << endl;
        return;
      }
      vec.erase(vec.begin() + e);
    }

    void print(){
      for(auto val : vec){
        cout << val << ", ";
      }
      cout << endl;
    }

    void addElem(int index, string e){
      if(index < 0 || index > vec.size()){
        cout << "bad index" << endl;
        return;
      }

      vec.insert(vec.begin() + index, e);
    }
};

int main() {

 // 1. Create initial vector with "Two"
 myArrayVector myVec({"Two"});

 // 2. Add elements at specific indices
 myVec.addElem(1, "Three");  // Add "Three" at index 1
 myVec.addElem(0, "One");    // Add "One" at index 0
 myVec.addElem(3, "Four");   // Add "Four" at index 3

 // 3. Print vector after insertions
 cout << "Vector after insertions: ";
 myVec.print();

 // 4. Remove element at index 0
 myVec.removeElem(0);

 // 5. Remove elements by value
 myVec.removeElem("Two");
 myVec.removeElem("Four");

 // 6. Insert name at index 1
 myVec.addElem(1, "Benjamin"); // Replace with your name

 // 7. Print using print() function
 cout << "Vector after modifications: ";
 myVec.print();

 // 8. Print using an iterator
 cout << "Vector printed using an iterator: ";
 for (auto it = myVec.vec.begin(); it != myVec.vec.end(); ++it) {
     cout << *it << " ";
 }
 cout << endl;

 return 0;
}