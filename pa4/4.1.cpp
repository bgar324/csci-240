#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
using namespace std;

bool isBalanced(string input){
  stack<string> s;
  stringstream tokens(input);
  string token;
  while(tokens >> token){
    if(token == "(" || token == "[" || token == "{"){
      s.push(token);
    } else if(token == ")" || token == "]" || token == "}"){
      if(s.empty()){
        cout <<  "invalid, no matching opening symbol" << endl;
        return false;
      }
      if((token == ")" && s.top()=="(") || (token == "]" && s.top()=="[") || (token == "}" && s.top()=="{")){
        s.pop();
      } else {
        cout << "invalid, closing symbol does not match opening symbol" << endl;
        return false;
      }
    }
  }
  if(!s.empty()){
    cout << "invalid, no matching closing symbol" << endl;
    return false;
  }
  cout << "valid" << endl;
  return true;
}

int main() {
  vector<string> tests = {
    "{ ( a + b ) * c1 }",
    "{ ( a + b ) * c1 ]",
    "( ( a + b ) * c1 ) / 15 )",
    "( ( ( a + b ) * c1 ) / 15 )",
    "a + b * c -",
    "( a + b / c -"
  };

  for (const string& test : tests) {
    cout << "Input: " << test << endl;
      isBalanced(test);
      cout << endl;
  }

  return 0;
}