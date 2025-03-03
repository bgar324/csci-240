#include <iostream>
using namespace std;

class Progression {
  protected:
    long current;

    // Advances the current value to the next value of the progression
    virtual void advance() {               // we declare this virtual, expecting it to be overridden
        current++;
    }

  public:
    // Constructs a progression with given start value (default: 0)
    Progression(long start = 0) : current{start} {}

    // Destructs the progression                                                                                                                   
    virtual ~Progression() {};             // virtual destructor

    // Returns the next value of the progression
    long next_value() {
        long answer{current};
        advance();                         // the protected call updates the current value
        return answer;
    }

    // Prints the next n values of the progression, separate by spaces
    void print_progression(int n) {
        cout << next_value();              // print the first value without leading space                                                      
        for (int j = 1; j < n; j++)
            cout << " " << next_value();   // print leading space before others                                                                
        cout << endl;
    }
};

class DiffProgression : public Progression {
  protected:
    long second_value_constructor;


    //goal, each progression is the absolute difference between the two previous values
    //example: 2,200,(|2-200| = 198), (|200-198| = 2), (|198-2|)
  virtual void advance(){
    long temp{second_value_constructor}; // sets 200 into a temp value
    second_value_constructor = abs(current - second_value_constructor); //ok svc = 198
    current = temp; //now current = 200
  }
  public:
    DiffProgression(long firstValue = 2, long secondValue = 200) : Progression{firstValue}, second_value_constructor(secondValue) {}
};

int main(){
  DiffProgression testCase1;
  testCase1.print_progression(5);

  DiffProgression testCase2(15,3);
  testCase2.print_progression(5);

  return 0;
}