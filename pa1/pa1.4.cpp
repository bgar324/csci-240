#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class CreditCard
{
private:
  string customer; // name of the customer (e.g., "John Bowman")
  string bank;     // name of the bank (e.g., "California Savings")
  string account;  // account identifier (e.g., "5391 0375 9387 5309")
  int limit;       // credit limit (measured in dollars)
  double interest; // new credit cards must carry an annual interest of 12 percent. 
  vector<double> purchases;
  vector<string> transactionTypes;

protected:
  double netCharges;
  double averageCharge;
  double highestCharge;
  vector<double> avgCalc;
  double avg;
  double balance; // current balance (measured in dollars)

public:
  // Constructs a new credit card. The initial balance is 0.0 by default.
  CreditCard(string cust, string bk, string acnt, int lim, double initial_bal = 0.0, double intrst = 12)
      : customer{cust}, bank{bk}, account{acnt}, limit{lim}, balance{initial_bal}, interest(intrst) {}

  // ------------- Accessors -------------
  string get_customer() const { return customer; }
  string get_bank() const { return bank; }
  string get_account() const { return account; }
  int get_limit() const { return limit; }
  double get_balance() const { return balance; }

  // ------------- Mutators -------------

  // Charges price to card if within credit limit, and returns whether successful
  bool charge(double price)
  {
    if (price + balance > limit) // if charge would surpass limit
      return false;              // refuse the charge
    // at this point, the charge is successful
    balance += price; // update the balance
    purchases.push_back(price);
    transactionTypes.push_back("charge");

    netCharges += price; // update netCharges as balance != netCharges
    avgCalc.push_back(price);

    //calculate the highest charge
    if(price > highestCharge) highestCharge = price;
    
    return true;      // announce the good news
  }

  // Processes customer payment to reduce the balance
  // store a counter, everytime this make payment is accessed, we increment this counter as well as add that payment value onto the list
  void make_payment(double amount){
    purchases.push_back(amount);
    balance -= amount; 
    transactionTypes.push_back("payment");
  }

  void display_purchases(){
    for(int i = 0; i < purchases.size(); i++){
      cout << purchases[i] << ", ";
    }
  }

  void display_transactions(){
    for(int i = 0; i < transactionTypes.size(); i++){
      cout << transactionTypes[i] << ", ";
    }
  }

  void display_history(){
    cout << "Transactions for " << customer << ":\n";
    cout << "-----------------------------------\n";
    for(int i = 0; i < purchases.size(); i++){
        cout << transactionTypes[i] << ": $" << purchases[i] << endl;
    }
    cout << "-----------------------------------\n";
}


  void addInterest(){
    double interestAmount = balance * (interest / 12 / 100);
    balance += interestAmount;
    purchases.push_back(interestAmount);
    transactionTypes.push_back("interest");
  } 

  void setZero(){
    netCharges = 0;
    highestCharge = 0;
    averageCharge = 0;
  }

  void calcAverage(){
    if (avgCalc.empty()) {
      avg = 0;  
  } else {
      avg = netCharges / avgCalc.size();
  }
  }

  // ---------- Overloaded output operator (as friend) ---------
  friend ostream &operator<<(ostream &out, const CreditCard &c)
  {
    out << "Customer = " << c.customer << endl;
    out << "Bank = " << c.bank << endl;
    out << "Account = " << c.account << endl;
    out << "Balance = " << c.balance << endl;
    out << "Limit = " << c.limit << endl;
    out << "Net Charges for month = " << c.netCharges << endl;
    out << "Highest Charge for the month = " << c.highestCharge << endl;
    out << "Average Charges for the month = " << c.avg << endl;
    return out;
  }
};

int main()
/*
private:
  string customer; // name of the customer (e.g., "John Bowman")
  string bank;     // name of the bank (e.g., "California Savings")
  string account;  // account identifier (e.g., "5391 0375 9387 5309")
  int limit;       // credit limit (measured in dollars)
  double interest; // new credit cards must carry an annual interest of 12 percent. 

protected:
  vector<double> purchases;
  double balance; // current balance (measured in dollars)
  */


{
  CreditCard testCase1 = CreditCard("Ben", "Chase", "1234", 1000);
  CreditCard testCase2 = CreditCard("Tim", "Bank of America", "5678", 600);

  /*
  loop 6 times for 6 months
    > make 1 or more charges: 1 -> 3 charges
    > charge = +
    > payment = - 
    > interest = +
  */

  //transactions are hardcoded for consistent testing as the prompt does not specify user input.


  vector<vector<double>> chargeAmounts = {
    {200.0, 150.0},    // Month 1
    {300.0},         // Month 2
    {120.0, 180.0},    // Month 3
    {50.0, 75.0, 125.0}, // Month 4
    {400.0},         // Month 5
    {350.0, 100.0}     // Month 6
  };

  vector<double> paymentAmounts = {150, 200, 100, 250, 300, 400};

  for(int i = 0; i < 6; i++){
    testCase1.setZero();
    testCase2.setZero();

    cout << "\nMonth " << (i + 1) << " Transactions:\n";

    //  testCase1

    //a
    for (double charge : chargeAmounts[i]) {
        testCase1.charge(charge);
    }

    //b
    testCase1.make_payment(paymentAmounts[i]);

    //c
    testCase1.addInterest();

    //d
    testCase1.calcAverage();
    cout << testCase1 << endl;

    // testCase2

    //a
    for (double charge : chargeAmounts[i]) {
        testCase2.charge(charge);
    }

    //b
    testCase2.make_payment(paymentAmounts[i]);

    //c
    testCase2.addInterest();

    //d
    testCase2.calcAverage();
    cout << testCase2 << endl;
}

  
  cout << "test case 1" << endl;
  testCase1.display_history();
  cout << "\n" << endl;

  cout << "test case 2" << endl;
  testCase2.display_history();
  return 0;
}