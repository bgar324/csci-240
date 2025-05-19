#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "../DSAC-main/priority/heap_priority_queue.h"
#include "../DSAC-main/tree/linked_binary_tree.h"
using namespace std;
using namespace dsac::priority;
using namespace dsac::tree;

class HuffmanEntry {
public:
    int frequency;
    LinkedBinaryTree<pair<char, int>> tree;
    
    HuffmanEntry(int freq, LinkedBinaryTree<pair<char, int>> t) : frequency(freq), tree(t) {}
    
    bool operator<(const HuffmanEntry& other) const {
        return frequency < other.frequency;
    }
};

class HuffmanTree {
private:
    LinkedBinaryTree<pair<char, int>> tree;
    unordered_map<char, string> codes;
    unordered_map<char, int> charFrequencies;
    
    void generateCodes(typename LinkedBinaryTree<pair<char, int>>::Position p, string code) {
        if (p.is_external()) {
            if (p.element().first != '\0') {
                codes[p.element().first] = code.empty() ? "0" : code;
            }
            return;
        }
        
        if (!p.left().is_null()) {
            generateCodes(p.left(), code + "0");
        }
        
        if (!p.right().is_null()) {
            generateCodes(p.right(), code + "1");
        }
    }
    
public:
    HuffmanTree(const string& text) {
        for (char c : text) {
            charFrequencies[c]++;
        }
        
        HeapPriorityQueue<HuffmanEntry> pq;
        
        for (const auto& [c, freq] : charFrequencies) {
            LinkedBinaryTree<pair<char, int>> t;
            t.add_root(make_pair(c, freq));
            pq.insert(HuffmanEntry(freq, t));
        }
        
        if (pq.size() == 1) {
            tree = pq.min().tree;
            codes[tree.root().element().first] = "0";
            return;
        }
        
        while (pq.size() > 1) {
            HuffmanEntry e1 = pq.min();
            pq.remove_min();
            HuffmanEntry e2 = pq.min();
            pq.remove_min();
            
            LinkedBinaryTree<pair<char, int>> newTree;
            newTree.add_root(make_pair('\0', e1.frequency + e2.frequency));
            newTree.attach(newTree.root(), e1.tree, e2.tree);
            
            pq.insert(HuffmanEntry(e1.frequency + e2.frequency, newTree));
        }
        
        if (!pq.empty()) {
            tree = pq.min().tree;
            generateCodes(tree.root(), "");
        }
    }
    
    string getCode(char c) const {
        auto it = codes.find(c);
        if (it != codes.end()) {
            return it->second;
        }
        return "";
    }
    
    string encode(const string& text) const {
        string result;
        for (char c : text) {
            result += getCode(c);
        }
        return result;
    }
    
    int getTotalBits(const string& text) const {
        int totalBits = 0;
        for (char c : text) {
            totalBits += getCode(c).length();
        }
        return totalBits;
    }
    
    void printTree() const {
        cout << "Huffman coding tree:" << endl;
        
        function<void(typename LinkedBinaryTree<pair<char, int>>::Position, int)> printNode = 
            [&](typename LinkedBinaryTree<pair<char, int>>::Position p, int indent) {
                if (p.is_null()) return;
                
                cout << string(indent, ' ');
                
                if (p.is_external()) {
                    cout << p.element().second << " ";
                    if (p.element().first == ' ') {
                        cout << "' '" << endl;
                    } else if (p.element().first == '_') {
                        cout << "'_'" << endl;
                    } else {
                        cout << p.element().first << endl;
                    }
                } else {
                    cout << p.element().second << " *" << endl;
                    printNode(p.left(), indent + 2);
                    printNode(p.right(), indent + 2);
                }
            };
        
        printNode(tree.root(), 0);
    }
    
    void printTreeAlt() const {
        cout << "Huffman coding tree:" << endl;
        
        function<void(typename LinkedBinaryTree<pair<char, int>>::Position, int)> printNode = 
            [&](typename LinkedBinaryTree<pair<char, int>>::Position p, int indent) {
                if (p.is_null()) return;
                
                cout << string(indent, ' ');
                
                if (p.is_external()) {
                    if (p.element().first == ' ') {
                        cout << "' '" << endl;
                    } else if (p.element().first == '_') {
                        cout << "'_'" << endl;
                    } else {
                        cout << p.element().first << endl;
                    }
                } else {
                    cout << "*" << endl;
                    printNode(p.left(), indent + 2);
                    printNode(p.right(), indent + 2);
                }
            };
        
        printNode(tree.root(), 0);
    }
    
    void printEncodingTable() const {
        vector<pair<char, int>> sortedChars(charFrequencies.begin(), charFrequencies.end());
        sort(sortedChars.begin(), sortedChars.end());
        
        cout << "\nChar\tFrequency\tEncoded bits" << endl;
        for (const auto& [c, freq] : sortedChars) {
            cout << (c == ' ' ? "' '" : (c == '_' ? "'_'" : string(1, c))) << "\t"
                 << freq << "\t\t"
                 << getCode(c) << endl;
        }
    }
    
    void printEncodedMessage(const string& text) const {
        string encodedMessage = encode(text);
        int totalBits = getTotalBits(text);
        
        cout << "\nNumber of bits to encode message: " << totalBits << endl;
        cout << encodedMessage << endl;
    }
};

int main() {
    string input;
    cout << "Enter text: ";
    getline(cin, input);
    
    cout << "Text: \"" << input << "\"" << endl;
    cout << "Number of characters: " << input.length() << endl << endl;
    
    HuffmanTree huffman(input);
    
    huffman.printTree();
    
    // Print encoding table and encoded message (extra credit)
    huffman.printEncodingTable();
    huffman.printEncodedMessage(input);
    
    return 0;
}