  /*
    Suppose you are given an array, A, containing n distinct integers that are
    listed in increasing order. Given a number k, provide a recursive function/method to find
    two integers in A that sum to k, if such a pair exists. For example, if A = {3, 9, 12, 15, 16,
    23} and k = 25 it outputs values 9 and 16. On the other hand, if k = 16 it outputs no pair
    exists. Output the results and the number of recursive calls for those 2 cases.
  */


  #include <iostream>
  #include <vector>
  #include <string>
  using namespace std;

  /*
  thought process:
  we could declare A[0] to be a part of the pair, then we can construct an array A[1] -> A[k-1], lets call this array B. then we can recursively call findKSum(B, k-A[0]). yeah, i dont know
  i guess what im thinking is that we set something to be the pair, then we recursively go through the array and see if that value at A[0] + A[i] = sum, then if that doesnt happen then we can do okay, part of the pair is then A[1]. the base case is then if we are ever at the end of the array and we havent broken out yet, then we output: "no pair exists"
  */

  string findKSum(vector<int>& A, int k) {
    static int left = 0;
    static int right = A.size() - 1;
    static int numRecursions = 0;
    numRecursions++;

    if (left >= right) {
      cout << "number of recrusions: " << numRecursions << endl;
      left = 0, right = A.size() - 1; 
      numRecursions = 0;
      return "no pair exists";
      numRecursions = 0;
    }

    int sum = A[left] + A[right];

    if (sum == k) {
        string result = "pair: " + to_string(A[left]) + " and " + to_string(A[right]);
        cout << "number of recrusions: " << numRecursions << endl;
        left = 0, right = A.size() - 1; 
        numRecursions = 0;
        return result;
    }
    else if (sum < k) {
        left++; 
    }
    else {
        right--; 
    }

    return findKSum(A, k); 
  }

  //hm ok
  //have two pointers, if left is greater than right, meaning they virtually swap positions
  // then something went wrong, and we should jump out
  // if this doesnt occur, check if the sum of the supposed two pointers are less than the k ?
  // if soo
  // uhhhhhhh, increase left, because then we can increase sum
  // vice versa for less than, make sum smaller
  // static ints to keep the data the same throughout the recursion :P

  int main() {
    vector<int> arr = {3, 9, 12, 15, 16, 23};

    cout << "For k = 25: " << endl << findKSum(arr, 25) << endl;
    cout << endl;
    cout << "For k = 16: " << endl << findKSum(arr, 16) << endl;

    return 0;
  }