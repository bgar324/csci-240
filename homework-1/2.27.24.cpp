#include <iostream>

void unreachableCode() {
    int x = 10;
    
    if (x > 100) { 
        std::cout << "This will never be printed.\n";
    }

    return;

    std::cout << "This statement is also unreachable!\n";
}

int main(){
  unreachableCode(); // this will never return anything due to logical fallacies.
  
  return 0;
}
