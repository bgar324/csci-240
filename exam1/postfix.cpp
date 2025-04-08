#include <iostream>
#include <stack>
#include <string>
#include <sstream>
using namespace std;

int main(){
  string input = "8 3 / 4 +";

  stringstream ss(input);
  stack<int> master;
  string token;

  int a = 0, b = 0;

  while(ss >> token){
    if(token == "+" || token == "-" || token == "*" || token == "/"){
      a = master.top();
      master.pop();
      b = master.top(); 
      master.pop();
      if(token == "+"){
        master.push(b+a);
      }else if(token == "-"){
        master.push(b-a);
      }else if(token == "*"){
        master.push(b*a);
      }else if(token == "/"){
        master.push(b/a);
      }
    } else {
      master.push(stoi(token));
    }
  }

  cout << master.top();

  return 0;
}