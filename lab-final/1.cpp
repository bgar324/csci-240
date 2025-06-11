#include <iostream>
#include <stdexcept>
using namespace std;

template<typename T>
struct MySNode{
    T element;
    MySNode<T>* next;
    MySNode(const T& e, MySNode<T>* n = nullptr) : element(e), next(n) {}
};

template<typename T>
class MySLList {
private:
    MySNode<T>* head;

public:
    // constructor: start with an empty list
    MySLList(): head(nullptr) { }

    // destructor: delete all nodes
    ~MySLList() {
        while (head  != nullptr) {
            MySNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // check if list is empty ( O(1) )
    bool empty() const {
        return head == nullptr;
    }

    // return number of elements ( O(n) )
    int size() const {
        int count = 0;
        MySNode<T>* p = head;
        while (p != nullptr) {
            ++count;
            p = p->next;
        }
        return count;
    }

    //print all elements (seperated by spaces and a new line) | O(n)
    void print() const {
        MySNode<T>* p = head;
        while (p != nullptr){
            cout << p->element;
            if(p->next != nullptr) {
                cout <<  " ";
            }
            p = p->next;
        }
        cout << "\n";
    }

    // insert e at the rear of the list [ O(1) best & O(n) worst ]
    void insertRear (const T& e){
        MySNode<T>* newNode = new MySNode<T>(e);
        // edge case
        if (head == nullptr){
            head = newNode;
            return;
        }
        MySNode<T>* p = head;
        // traversing
        while (p->next != nullptr){
            p = p->next;
        }
        p->next = newNode;
    }

    // remove and return the element at the rear. if empty, throw error [ O(n) ]
    T removeRear(){
        if(head == nullptr){
            throw runtime_error("removeRear from empty list");
        }
        // if only 1 node
        if(head->next == nullptr){
            T val = head->element;
            delete head;
            head = nullptr;
            return val;
        }
        // otherwise, find second to last node.
        MySNode<T>* p = head;
        while (p->next->next != nullptr) {
            p = p->next;
        }
        // now. p->next is the last node.
        T val = p->next->element;
        delete p->next;
        p->next = nullptr;
        return val;
    }

    // remove 1st occurence of e, return true if removed & false if not [ O(n) ]
    bool removeElem(const T& e){
        if(head == nullptr) return false;
        // if head holds e...
        if(head->element == e){
            MySNode<T>* temp = head;
            head = head->next;
            delete temp;
            return true;
        }

        //otherwise, find a node whose next holds e
        MySNode<T>* p = head;
        while(p->next != nullptr && p->next->element != e){
            p = p->next;
        }
        if(p->next == nullptr){
            return false; //not found
        }
        // good case: p->next holds e
        MySNode<T>* toDelete = p->next;
        p->next = toDelete->next;
        delete toDelete;
        return true;
    }
};

int main() {
    cout << "==== Test MySLList<int> ====\n";
    MySLList<int> intList;
    cout << "Empty? " << boolalpha << intList.empty() << "\n";
    intList.insertRear(5);
    intList.insertRear(10);
    intList.insertRear(15);
    cout << "After inserting 5,10,15: ";
    intList.print();                  // prints "5 10 15"
    cout << "Size: " << intList.size() << "\n";  // 3
    bool removed = intList.removeElem(10);
    cout << "removeElem(10)? " << boolalpha << removed << "\n";
    cout << "List now: ";
    intList.print();                  // prints "5 15"
    int x = intList.removeRear();
    cout << "removeRear() returned " << x << "\n"; // 15
    cout << "List now: ";
    intList.print();                  // prints "5"
    cout << "Empty? " << boolalpha << intList.empty() << "\n"; // false

    cout << "\n==== Test MySLList<string> ====\n";
    MySLList<string> strList;
    strList.insertRear("hello");
    strList.insertRear("world");
    strList.insertRear("!!");
    cout << "After inserting \"hello\",\"world\",\"!!\": ";
    strList.print();                  // prints "hello world !!"
    cout << "Size: " << strList.size() << "\n";  // 3
    bool rem2 = strList.removeElem("world");
    cout << "removeElem(\"world\")? " << boolalpha << rem2 << "\n";
    cout << "List now: ";
    strList.print();                  // prints "hello !!"
    string s = strList.removeRear();
    cout << "removeRear() returned \"" << s << "\"\n"; // "!!"
    cout << "List now: ";
    strList.print();                  // prints "hello"
    cout << "Empty? " << boolalpha << strList.empty() << "\n"; // false

    return 0;
}