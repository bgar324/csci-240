/*
  Implement the two algorithms for prefix averages from your textbook. Run
  your program with the 10 values (31 -41 59 26 -53 58 97 -93 -23 84) as one test case to
  confirm that it is working correctly and output the results to the screen. In addition, run
  your program and output only running times in ms or ns for 100, 1000, 10000, and
  100000 elements (generate random values between -10000 and 10000; if you have a very
  fast computer, you might want to try 1000000 values). Do confirm that the collected
  running times match the expected running times in Big-O notation of O(n^2) and O(n)
  for the two algorithms, prefix averages 1 and prefix averages 2.
*/


#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>

using namespace std;
using namespace chrono;

// Computes prefix averages such that result a[j] equals the average of x[0], ..., x[j].
vector<double> prefix_averages1(const vector<double>& x) {
  int n(x.size());
  vector<double> a(n);              // filled with n zeros by default                                                                                        
  for (int j = 0; j < n; j++) {
      double total{0.0};            // begin computing x[0] + ... + x[j]                                                                                     
      for (int i = 0; i <= j; i++)
          total += x[i];
      a[j] = total / (j + 1);       // record the average                                                                                                    
  }
  return a;
}

// Computes prefix averages such that result a[j] equals the average of x[0], ..., x[j].
vector<double> prefix_averages2(const vector<double>& x) {
  int n(x.size());
  vector<double> a(n);              // filled with n zeros by default                                                                                        
  double total{0};                  // compute prefix sum as x[0] + x[1] + ...                                                                               
  for (int j = 0; j < n; j++) {
      total += x[j];                // update prefix sum to include x[j]                                                                                     
      a[j] = total / (j + 1);       // computer average based on current sum                                                                                 
  }
  return a;
}

//helper function to print out a vector. dont know if ill need this for the large test cases
void printVector(const vector<double>& v) {
  cout << "[ ";
  for (int i = 0; i < v.size(); i++) {
      cout << v[i];
      if (i < v.size() - 1) cout << ", ";  
  }
  cout << " ]" << endl;
}

//helper function to generate a pure random vector
vector<double> generateRandomVector(int size){
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> dist(-10000, 10000);

  vector<double> vec(size);
  for (int i = 0; i < size; i++) {
      vec[i] = dist(gen);  // grab a random number
  }

  return vec;
}

//helper to measure time
void measureTime(const vector<double>& vec, const string& functionName, bool isOptimized) {
  auto start = high_resolution_clock::now();  // Start timer
  
  if (isOptimized) {
      prefix_averages2(vec);
  } else {
      prefix_averages1(vec);
  }
  
  auto stop = high_resolution_clock::now(); 
  auto duration = duration_cast<nanoseconds>(stop - start).count();  // Convert to ms

  cout << functionName << " took " << duration << " ns." << endl;
}

int main(){

  vector<double> initialTestCase = {31, -41, 59, 26, -53, 58, 97, -93, -23, 84};

  cout << "prefix average 1 test" << endl;
  printVector(prefix_averages1(initialTestCase));

  cout << "prefix average 2 test" << endl;
  printVector(prefix_averages2(initialTestCase));

  //test cases

  vector<double> vectorHundred = generateRandomVector(100);
  vector<double> vectorThousand = generateRandomVector(1000);
  vector<double> vectorTenThousand = generateRandomVector(10000);
  vector<double> vectorHundredThousand = generateRandomVector(100000);

  //measuring time
  measureTime(vectorHundred, "prefix_averages1 (100 elements)", false);
  measureTime(vectorHundred, "prefix_averages2 (100 elements)", true);

  measureTime(vectorThousand, "prefix_averages1 (1000 elements)", false);
  measureTime(vectorThousand, "prefix_averages2 (1000 elements)", true);

  measureTime(vectorTenThousand, "prefix_averages1 (10000 elements)", false);
  measureTime(vectorTenThousand, "prefix_averages2 (10000 elements)", true);

  measureTime(vectorHundredThousand, "prefix_averages1 (100000 elements)", false);
  measureTime(vectorHundredThousand, "prefix_averages2 (100000 elements)", true);

  return 0;
}