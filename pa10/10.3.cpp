#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>
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
    
    void generateCodes(typename LinkedBinaryTree<pair<char, int>>::Position p, string code) {
        if (p.is_external()) {
            if (p.element().first != '\0') {
                codes[p.element().first] = code;
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
        unordered_map<char, int> frequencies;
        for (char c : text) {
            frequencies[c]++;
        }
        
        HeapPriorityQueue<HuffmanEntry> pq;
        
        for (const auto& [c, freq] : frequencies) {
            LinkedBinaryTree<pair<char, int>> t;
            t.add_root(make_pair(c, freq));
            pq.insert(HuffmanEntry(freq, t));
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
        cout << "Alt Huffman coding tree:" << endl;
        
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
};

int main() {
    string input;
    cout << "Enter text: ";
    getline(cin, input);
    
    cout << "Text: \"" << input << "\"" << endl;
    cout << "Number of characters: " << input.length() << endl << endl;
    
    HuffmanTree huffman(input);
    
    huffman.printTree();
    
    cout << endl;
    
    huffman.printTreeAlt();
    
    return 0;
}