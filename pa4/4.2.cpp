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

// EXTRA CREDIT //
int precedence(char op){
  if(op == '+' || op == '-'){
    return 1;
  } else if (op == '*' || op == '/' || op == '%'){
    return 2;
  } else {
    return 0;
  }
}

string infixToPostfix(string infix){
  MyArrayStack<char> opStack;
  stringstream ss(infix);
  string token;
  string output = "";

  while(ss >> token){
    if(isdigit(token[0]) || (token.length() > 1 && token[0] == '-' && isdigit(token[1]))){
      output += token + " ";
    } else if (token == "("){
      opStack.push('(');
    } else if(token == ")"){
      while(!opStack.empty() && opStack.top() != '('){
        output += opStack.pop();
        output += " ";
      }
      if(!opStack.empty()){
        opStack.pop();
      }
    } else {
      while(!opStack.empty() && precedence(opStack.top()) >= precedence(token[0])){
        output += opStack.pop();
        output += " ";
      }
      opStack.push(token[0]);
    }
  }
  while(!opStack.empty()){
    output += opStack.pop();
    output += " ";
  }
  return output;
}

int main(){

  cout << evaluatePostfix("17 2 3 + / 13 -") << endl;   // -10
  cout << evaluatePostfix("5 2 3 + *") << endl;         // 25
  cout << evaluatePostfix("2 3 2 % *") << endl;           // 2
  cout << evaluatePostfix("-23 123 + 2 *") << endl;     // 200

  cout << infixToPostfix("17 / ( 2 + 3 ) - 13") << endl;   // 17 2 3 + / 13 -
  cout << infixToPostfix("5 * ( 2 + 3 )") << endl;         // 5 2 3 + *
  cout << infixToPostfix("2 * ( 3 % 2 )") << endl;         // 2 3 2 % *
  cout << infixToPostfix("( -23 + 123 ) * 2") << endl;     // -23 123 + 2 *

  return 0;
}