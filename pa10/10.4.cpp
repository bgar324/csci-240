#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

vector<vector<int>> lcs(string& X, string& Y) {
    int n(X.size());
    int m(Y.size());
    vector<vector<int>> L(n + 1, vector<int>(m + 1));    // (n+1) x (m+1) table
    for (int j = 0; j < n; j++)
        for (int k = 0; k < m; k++)
            if (X[j] == Y[k])                            // align this match
                L[j + 1][k + 1] = L[j][k] + 1;
            else                                         // choose to ignore one character
                L[j + 1][k + 1] = max(L[j][k + 1], L[j + 1][k]);
    return L;
}

// Function to print the LCS table
void printLCSTable(const vector<vector<int>>& L, const string& X, const string& Y) {
    int n = X.size();
    int m = Y.size();
    
    // Print header row with Y characters
    cout << "    ";
    cout << "  ";  // Empty cell for row headers
    for (int k = 0; k < m; k++) {
        cout << Y[k] << " ";
    }
    cout << endl;
    
    // Print the table with row headers
    for (int j = 0; j <= n; j++) {
        if (j == 0) {
            cout << "  ";  // Empty cell for the first row
        } else {
            cout << X[j-1] << " ";  // Print X character as row header
        }
        
        for (int k = 0; k <= m; k++) {
            cout << L[j][k] << " ";
        }
        cout << endl;
    }
}

// Function to reconstruct the longest common subsequence
string reconstructLCS(const vector<vector<int>>& L, const string& X, const string& Y) {
    string result;
    int j = X.size();
    int k = Y.size();
    
    while (j > 0 && k > 0) {
        if (X[j-1] == Y[k-1]) {
            // Characters match, add to result and move diagonally
            result.push_back(X[j-1]);
            j--;
            k--;
        } else if (L[j-1][k] >= L[j][k-1]) {
            // Move up
            j--;
        } else {
            // Move left
            k--;
        }
    }
    
    // Reverse the result since we built it backwards
    reverse(result.begin(), result.end());
    return result;
}

int main() {
    // Test case 1
    string X1 = "GTTCCTAATA";
    string Y1 = "CGATAATTGAGA";
    
    cout << "Test Case 1:" << endl;
    cout << "X = \"" << X1 << "\"" << endl;
    cout << "Y = \"" << Y1 << "\"" << endl;
    
    vector<vector<int>> L1 = lcs(X1, Y1);
    
    cout << "\nLCS Table:" << endl;
    printLCSTable(L1, X1, Y1);
    
    string lcsResult1 = reconstructLCS(L1, X1, Y1);
    cout << "\nLongest Common Subsequence: \"" << lcsResult1 << "\"" << endl;
    cout << "Length: " << lcsResult1.length() << endl;
    
    cout << "\n----------------------------------------\n" << endl;
    
    // Test case 2
    string X2 = "ACTIVE";
    string Y2 = "RELATIVELY";
    
    cout << "Test Case 2:" << endl;
    cout << "X = \"" << X2 << "\"" << endl;
    cout << "Y = \"" << Y2 << "\"" << endl;
    
    vector<vector<int>> L2 = lcs(X2, Y2);
    
    cout << "\nLCS Table:" << endl;
    printLCSTable(L2, X2, Y2);
    
    string lcsResult2 = reconstructLCS(L2, X2, Y2);
    cout << "\nLongest Common Subsequence: \"" << lcsResult2 << "\"" << endl;
    cout << "Length: " << lcsResult2.length() << endl;
    
    return 0;
}
