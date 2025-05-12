#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

// BRUTE FORCE
// Returns the lowest index at which substring pattern begins in text (or else -1)
int find_brute(const string& text, const string& pattern) {
    int n(text.size());
    int m(pattern.size());
    for (int i = 0; i <= n - m; i++) {                   // try every starting index within text
        int k{0};                                        // k is index into pattern
        while (k < m && text[i+k] == pattern[k])         // kth character of pattern matches
            k++;
        if (k == m)                                      // if we reach the end of the pattern,
            return i;                                    // substring text[i..i+m-1] is a match
    }
    return -1;                                           // search failed
}

// BOYER-MOORE
// Returns the lowest index at which substring pattern begins in text (or else -1).
int find_boyer_moore(const string& text, const string& pattern) {
    int n(text.size());
    int m(pattern.size());
    if (m == 0) return 0;                                // trivial search for empty string
    unordered_map<char,int> last;                        // the 'last' map
    for (int i = 0; i < n; i++)
        last[text[i]] = -1;                              // set -1 as default for all text characters
    for (int k = 0; k < m; k++)
        last[pattern[k]] = k;                            // rightmost occurrence in pattern is last
    // start with the end of the pattern aligned at index m-1 of the text
    int i{m - 1};                                        // an index into the text
    int k{m - 1};                                        // an index into the pattern
    while (i < n) {
        if (text[i] == pattern[k]) {                     // a matching character
            if (k == 0) return i;                        // entire pattern has been found
            i--;                                         // otherwise, examine previous
            k--;                                         // characters of text/pattern
        } else {
            i += m - min(k, 1 + last[text[i]]);          // case analysis for jump step
            k = m - 1;                                   // restart at end of pattern
        }
    }
    return -1;                                           // pattern was never found
}

// KNUTH UTILITY CLASS
// Returns the failure table for Knuth, Morris Pratt algorithm
vector<int> compute_kmp_fail(const string& pattern) {
    int m(pattern.size());
    vector<int> fail(m,0);                               // by default, all overlaps are zero
    int j{1};
    int k{0};
    while (j < m) {                                      // compute fail[i] during this pass, if nonzero
        if (pattern[j] == pattern[k]) {                  // k + 1 characters match thus far
            fail[j] = k + 1;
            j++;
            k++;
        } else if (k > 0)                                // k follows a matching prefix
            k = fail[k - 1];
        else                                             // no match found starting at j
            j++;
    }
    return fail;
}

// KNUTH-MORRIS-PRATT
// Returns the lowest index at which substring pattern begins in text (or else -1).
int find_kmp(const string& text, const string& pattern) {
    int n(text.size());
    int m(pattern.size());
    if (m == 0) return 0;                                // trivial search for empty string
    vector<int> fail{compute_kmp_fail(pattern)};         // computed by separate utility function
    int i{0};                                            // index into text
    int k{0};                                            // index into pattern
    while (i < n) {
        if (text[i] == pattern[k]) {                     // pattern[0..k] matched thus far
            if (k == m - 1) return i - m + 1;            // match is complete
            i++;                                         // otherwise, try to extend match
            k++;
        } else if (k > 0)
            k = fail[k - 1];                             // reuse suffix of P[0..k-1]
        else
            i++;
    }
    return -1;                                           // reached end without match
}

// --- WRAPPER FUNCTIONS TO COUNT COMPARISONS ---

int brute_force_with_count(const string& text, const string& pattern, int& compares) {
    int n(text.size());
    int m(pattern.size());
    for (int i = 0; i <= n - m; i++) {
        int k{0};
        while (k < m) {
            compares++;
            if (text[i+k] != pattern[k])
                break;
            k++;
        }
        if (k == m)
            return i;
    }
    return -1;
}

int boyer_moore_with_count(const string& text, const string& pattern, int& compares) {
    int n(text.size());
    int m(pattern.size());
    if (m == 0) return 0;
    unordered_map<char,int> last;
    for (int i = 0; i < n; i++)
        last[text[i]] = -1;
    for (int k = 0; k < m; k++)
        last[pattern[k]] = k;
    int i{m - 1};
    int k{m - 1};
    while (i < n) {
        compares++;
        if (text[i] == pattern[k]) {
            if (k == 0) return i;
            i--;
            k--;
        } else {
            i += m - min(k, 1 + last[text[i]]);
            k = m - 1;
        }
    }
    return -1;
}

int kmp_with_count(const string& text, const string& pattern, int& compares) {
    int n(text.size());
    int m(pattern.size());
    if (m == 0) return 0;
    vector<int> fail{compute_kmp_fail(pattern)};
    int i{0};
    int k{0};
    while (i < n) {
        compares++;
        if (text[i] == pattern[k]) {
            if (k == m - 1) return i - m + 1;
            i++;
            k++;
        } else if (k > 0)
            k = fail[k - 1];
        else
            i++;
    }
    return -1;
}

// driver
void run_test(const string& T, const string& P, const string& label) {
    int compares;

    cout << "Test: " << label << endl;

    compares = 0;
    int idx = brute_force_with_count(T, P, compares);
    cout << "  Brute Force:      Index = " << idx << ", Comparisons = " << compares << endl;

    compares = 0;
    idx = boyer_moore_with_count(T, P, compares);
    cout << "  Boyer-Moore:      Index = " << idx << ", Comparisons = " << compares << endl;

    compares = 0;
    idx = kmp_with_count(T, P, compares);
    cout << "  KMP:              Index = " << idx << ", Comparisons = " << compares << endl;

    cout << endl;
}

int main() {
// 1. T: "aaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbb", P: "aaaab", P: "aabaa"
    string T1 = "aaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbb";
    run_test(T1, "aaaab", "T1, P = \"aaaab\"");
    run_test(T1, "aabaa", "T1, P = \"aabaa\"");

    // 2. T: "a pattern matching algorithm", P: "rithm", P: "rithn"
    string T2 = "a pattern matching algorithm";
    run_test(T2, "rithm", "T2, P = \"rithm\"");
    run_test(T2, "rithn", "T2, P = \"rithn\"");

    // 3. T: "GTTTATGTAGCTTACCTCCTCAAAGCAATACACTGAAAA", P: "CTGA", P: "CTGG"
    string T3 = "GTTTATGTAGCTTACCTCCTCAAAGCAATACACTGAAAA";
    run_test(T3, "CTGA", "T3, P = \"CTGA\"");
    run_test(T3, "CTGG", "T3, P = \"CTGG\"");

    return 0;
}