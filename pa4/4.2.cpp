#include <iostream>
#include <stack>
#include <string>
#include <sstream>
using namespace std;

template <typename T>
class MyArrayStack{
  private:
    T data[10];
    int topIndex;
  public:
    //constructor
    MyArrayStack(){
      topIndex = -1; // <- starts empty
    }
    int size(){
      return topIndex + 1;
    }
    bool empty(){
      return topIndex == -1;
    }
    T top(){
      if(topIndex == -1){
        cout << "stack is empty. please add onto the array before you use this function." << endl;
        exit(1);
      }
      return data[topIndex];
    }
    void push(T e){
      if(topIndex == 9){
        cout << "stack is full." << endl;
        exit(1);
      }
      data[++topIndex] = e;
    }

    T pop(){
      if(topIndex == -1){
        cout << "stack is empty." << endl;
        exit(1);
      }
      return data[topIndex--]; //returns topindex then decerments in the same step to 'forget' whats at topindex
    }
};

int evaluatePostfix(string expression) {
    MyArrayStack<int> s;
    stringstream ss(expression);
    string token;

    while (ss >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%") {
            int b = s.pop();
            int a = s.pop();
            int result;

            if (token == "+") result = a + b;
            else if (token == "-") result = a - b;
            else if (token == "*") result = a * b;
            else if (token == "/") result = a / b;
            else if (token == "%") result = a % b;

            s.push(result);
        } 
        else {
            s.push(stoi(token)); // convert token to int and push
        }
    }

    return s.pop(); // final result
}

int main(){

  cout << evaluatePostfix("17 2 3 + / 13 -") << endl;   // -10
  cout << evaluatePostfix("5 2 3 + *") << endl;         // 25
  cout << evaluatePostfix("2 3 8 *") << endl;           // 2
  cout << evaluatePostfix("-23 123 + 2 *") << endl;     // 200

  return 0;
}