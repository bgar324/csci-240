#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
using namespace std;

template <typename E>
class LinkedBinaryTree {
  protected:
    //------ nested Node class ------
    class Node {
      public:                   // members public for convenience, as Node class is protected
        E element;
        Node* parent;
        Node* left{nullptr};
        Node* right{nullptr};

        Node(E e, Node* p = nullptr) : element{e}, parent{p} {}
    };  // end of Node class

    //------ data members of LinkedBinaryTree ------
    Node* rt{nullptr};
    int sz{0};

  public:
    //------ nested Position class ------
    class Position {
      private:
        Node* node;
        friend class LinkedBinaryTree;    // allow outer class access to node pointer
        
      public:
        Position(Node* nd = nullptr) : node{nd} {}
                 
        bool operator==(Position other) const { return node == other.node; }
        bool operator!=(Position other) const { return node != other.node; }

        bool is_null() const { return node == nullptr; }
        bool is_root() const { return node->parent == nullptr; }
        bool is_external() const { return node->left == nullptr && node->right == nullptr; }
        
        E& element() { return node->element; }                 // will allow element to be edited
        const E& element() const { return node->element; }
        Position parent() const { return Position(node->parent); }
        Position left() const { return Position(node->left); }
        Position right() const { return Position(node->right); }
        
        vector<Position> children() const {
            vector<Position> result;
            if (node->left != nullptr) result.push_back(Position(node->left));
            if (node->right != nullptr) result.push_back(Position(node->right));
            return result;
        }
        
        int num_children() const {
            int result{0};
            if (node->left != nullptr) result++;
            if (node->right != nullptr) result++;
            return result;
        }
    };  // end of Position class

    //------ member functions of LinkedBinaryTree ------

    LinkedBinaryTree() {}                           // Constructs a tree with zero nodes

    int size() const { return sz; }                 // Returns the number of elements in the tree
    bool empty() const { return sz == 0; }          // True if the tree has size 0

    Position root() const { return Position(rt); }  // Returns a Position for the root (possibly null)

    vector<Position> positions() const {            // Returns an inorder sequence of positions
        vector<Position> gather;
        if (rt) inorder(Position(rt), gather);      // (the inorder function is defined in later section)
        return gather;
    }

    void add_root(const E& e = E()) {               // add root to (presumed) empty tree
        rt = new Node(e);
        sz = 1;
    }

    void add_left(Position p, const E& e) {         // Creates a new left child of p storing e
        p.node->left = new Node{e, p.node};         // parent of new node is p's node
        sz++;
    }
    
    void add_right(Position p, const E& e) {        // Creates a new left child of p storing e
        p.node->right = new Node{e, p.node};        // parent of new node is p's node
        sz++;
    }
    
    void erase(Position p) {                        // Removes p, promoting its one child, if any
        Node* nd = p.node;
        Node* child{nd->left == nullptr ? nd->right : nd->left};  // lone child or else nullptr
        
        if (child != nullptr)
            child->parent = nd->parent;             // child's grandparent becomes parent

        if (nd == rt)
            rt = child;                             // child promoted to root
        else if (nd->parent->left == nd)            // node is left child of its parent
            nd->parent->left = child;
        else                                        // node is right child of its parent
            nd->parent->right = child;
        sz--;
        delete nd;
    }

    // Attaches the internal structures of trees left and right and subtrees of leaf p.
    void attach(Position p, LinkedBinaryTree& left, LinkedBinaryTree& right) {
        Node* nd = p.node;
        nd->left = left.rt;
        nd->right = right.rt;
        sz += left.sz + right.sz;

        // adjust parent pointers for old roots (if they exist)
        if (left.rt) left.rt->parent = nd;
        if (right.rt) right.rt->parent = nd;

        // reset other trees to an empty state
        left.sz = right.sz = 0;
        left.rt = right.rt = nullptr;
    }

    // Helper function for positions() method
    void inorder(Position p, vector<Position>& result) const {
        if (p.left().node != nullptr)
            inorder(p.left(), result);
        result.push_back(p);
        if (p.right().node != nullptr)
            inorder(p.right(), result);
    }

  // ------------- Rule-of-five support ----------------
  private:
    void tear_down(Node* nd) {
        if (nd != nullptr) {
            tear_down(nd->left);
            tear_down(nd->right);
            delete nd;
        }
    }

    // Create cloned structure of model and return pointer to the new structure
    static Node* clone(Node* model) {
        if (model == nullptr) return nullptr;       // trivial clone
        Node* new_root = new Node(model->element);
        new_root->left = clone(model->left);
        if (new_root->left) new_root->left->parent = new_root;
        new_root->right = clone(model->right);
        if (new_root->right) new_root->right->parent = new_root;
        return new_root;
    }
    
  public:
    // destructor
    ~LinkedBinaryTree() { tear_down(rt); }

    // copy constructor
    LinkedBinaryTree(const LinkedBinaryTree& other) : sz{other.sz}, rt{clone(other.rt)} {}

    // copy assignment
    LinkedBinaryTree& operator=(const LinkedBinaryTree& other) {
        if (this != &other) {                       // bypass self-assignment
            tear_down(rt);
            rt = clone(other.rt);
            sz = other.sz;
        }
        return *this;
    }

    // move constructor
    LinkedBinaryTree(LinkedBinaryTree&& other) : sz{other.sz}, rt{other.rt} {
        // reset other to empty
        other.sz = 0;
        other.rt = nullptr;
    }

    // move assignment
    LinkedBinaryTree& operator=(LinkedBinaryTree&& other) {
        if (this != &other) {                       // bypass self-assignment
            using std::swap;
            swap(sz, other.sz);
            swap(rt, other.rt);                     // old structure will be destroyed by other
        }
        return *this;
    }
};  // end of LinkedBinaryTree class

template <typename Entry, typename Compare=less<Entry>>
class HeapPriorityQueue {
  protected:
    vector<Entry> data;                           // our underlying collection
    Compare less_than;                                     // less_than(a,b) defines "a < b" relationship

    int parent(int j) { return (j-1) / 2; }                // truncating division
    int left(int j) { return 2 * j + 1; }
    int right(int j) { return 2 * j + 2; }
    bool has_left(int j) { return left(j) < data.size(); }
    bool has_right(int j) { return right(j) < data.size(); }
    
    // Moves the entry at index j higher, if necessary, to restore the heap property
    void upheap(int j) {
        while (j > 0) {                                    // continue to root (or break statement)
            int p{parent(j)};
            if (less_than(data[j], data[p])) {             // if j's entry is less than its parent
                swap(data[j], data[p]);
                j = p;                                     // continue from new location after the swap
            } else
                break;                                     // j is properly placed relative to parent
        }
    }
    
    // Moves the entry at index j lower, if necessary, to restore the heap property
    void downheap(int j) {
        while (has_left(j)) {                              // continue to bottom (or break statement)
            int small_child{left(j)};                      // although right may be smaller
            if (has_right(j)) {
                if (less_than(data[right(j)], data[left(j)]))
                    small_child = right(j);
            }

            if (less_than(data[small_child], data[j])) {   // if small child's entry is less than j's
                swap(data[j], data[small_child]);
                j = small_child;                           // continue from new location after the swap
            } else
                break;
        }
    }

  public:
    HeapPriorityQueue() {}                                 // default is an empty priority queue

    int size() const { return data.size(); }               // returns number of entries
    bool empty() const { return data.empty(); }            // returns true if zero entries
    const Entry& min() const { return data.front(); }      // returns reference to root entry

    // Inserts a new entry into the priority queue
    void insert(const Entry& e) {
        data.push_back(e);                                 // add to the end of the vector
        upheap(data.size() - 1);                           // upheap newly added entry
    }
    
    // Removes the minimum entry
    void remove_min() {
        if (data.empty()) return;
        swap(data[0], data[data.size() - 1]);              // put last entry at the root
        data.pop_back();                                   // and remove the displaced minimum
        if (!data.empty())                                 // only downheap if not empty
            downheap(0);                                   // fix heap property for new root
    }
    
    // Returns and removes the minimum entry
    Entry extract_min() {
        Entry result = min();
        remove_min();
        return result;
    }
};

// Node class for Huffman tree
class HuffmanNode {
public:
    char character;
    int frequency;
    bool isInternal;
    
    HuffmanNode(char c, int freq, bool internal = false) 
        : character(c), frequency(freq), isInternal(internal) {}
    
    // For displaying in the tree
    string toString() const {
        if (isInternal) {
            return to_string(frequency) + " *";
        } else {
            if (character == ' ') {
                return to_string(frequency) + " _";  // Underscore for space
            } else if (character == '\n') {
                return to_string(frequency) + " \\n";  // Newline
            } else if (character == '_') {
                return to_string(frequency) + " _";  // Underscore
            } else {
                return to_string(frequency) + " " + character;
            }
        }
    }
    
    // Alternative display format (without frequency)
    string toStringNoFreq() const {
        if (isInternal) {
            return "*";
        } else {
            if (character == ' ') {
                return "_";  // Underscore for space
            } else if (character == '\n') {
                return "\\n";  // Newline
            } else if (character == '_') {
                return "_";  // Underscore
            } else {
                return string(1, character);
            }
        }
    }
};

class HuffmanTree {
private:
    LinkedBinaryTree<HuffmanNode*> tree;
    unordered_map<char, string> codes;
    
    // Helper function to compute character frequencies
    unordered_map<char, int> computeFrequencies(const string& text) {
        unordered_map<char, int> freqs;
        for (char c : text) {
            freqs[c]++;
        }
        return freqs;
    }
    
public:
    // Construct a Huffman tree from the given text
    HuffmanTree(const string& text) {
        // Compute character frequencies
        unordered_map<char, int> freqs = computeFrequencies(text);
        
        // Create a min heap priority queue
        auto compare = [](HuffmanNode* a, HuffmanNode* b) {
            return a->frequency > b->frequency;
        };
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, decltype(compare)> pq(compare);
        
        // Add leaf nodes for each character
        for (const auto& pair : freqs) {
            pq.push(new HuffmanNode(pair.first, pair.second));
        }
        
        // Special case: if there's only one unique character
        if (pq.size() == 1) {
            HuffmanNode* leaf = pq.top(); pq.pop();
            HuffmanNode* root = new HuffmanNode('\0', leaf->frequency, true);
            
            tree.add_root(root);
            tree.add_left(tree.root(), leaf);
            return;
        }
        
        // Build the tree by combining nodes
        while (pq.size() > 1) {
            HuffmanNode* left = pq.top(); pq.pop();
            HuffmanNode* right = pq.top(); pq.pop();
            
            // Create a new internal node with these two as children
            HuffmanNode* parent = new HuffmanNode('\0', left->frequency + right->frequency, true);
            
            // Create a new tree with the parent as root
            LinkedBinaryTree<HuffmanNode*> parentTree;
            parentTree.add_root(parent);
            
            // Create trees for left and right
            LinkedBinaryTree<HuffmanNode*> leftTree;
            leftTree.add_root(left);
            
            LinkedBinaryTree<HuffmanNode*> rightTree;
            rightTree.add_root(right);
            
            // Attach left and right subtrees to parent
            parentTree.attach(parentTree.root(), leftTree, rightTree);
            
            // If this is the last merge, this becomes our tree
            if (pq.empty()) {
                tree = parentTree;
            } else {
                // Otherwise, add the parent node back to the queue
                pq.push(parent);
            }
        }
        
        // Generate Huffman codes
        generateCodes(tree.root(), "");
    }
    
    // Helper function to generate Huffman codes
    void generateCodes(typename LinkedBinaryTree<HuffmanNode*>::Position node, const string& code) {
        if (node.is_null()) return;
        
        HuffmanNode* huffNode = node.element();
        
        // If it's a leaf node, store the code
        if (!huffNode->isInternal) {
            codes[huffNode->character] = code;
        }
        
        // Recursively generate codes for left (0) and right (1) children
        generateCodes(node.left(), code + "0");
        generateCodes(node.right(), code + "1");
    }
    
    // Print the Huffman tree
    void printTree(bool showFreq = true) {
        cout << "Huffman coding tree:" << endl;
        printTree(tree.root(), 0, showFreq);
    }
    
    // Helper function to print the tree with indentation
    void printTree(typename LinkedBinaryTree<HuffmanNode*>::Position node, int depth, bool showFreq) {
        if (node.is_null()) return;
        
        // Print indentation
        for (int i = 0; i < depth; i++) {
            cout << "    ";
        }
        
        // Print node
        if (showFreq) {
            cout << node.element()->toString() << endl;
        } else {
            cout << node.element()->toStringNoFreq() << endl;
        }
        
        // Print children
        printTree(node.left(), depth + 1, showFreq);
        printTree(node.right(), depth + 1, showFreq);
    }
    
    // Get the Huffman code for a character
    string getCode(char c) const {
        auto it = codes.find(c);
        if (it != codes.end()) {
            return it->second;
        }
        return "";
    }
    
    // Encode a text using the Huffman codes
    string encode(const string& text) const {
        string encoded;
        for (char c : text) {
            encoded += getCode(c);
        }
        return encoded;
    }
    
    // Cleanup memory
    ~HuffmanTree() {
        // Clean up dynamically allocated HuffmanNodes
        vector<typename LinkedBinaryTree<HuffmanNode*>::Position> positions = tree.positions();
        for (auto pos : positions) {
            delete pos.element();
        }
    }
};

int main() {
    string text;
    cout << "Enter text: ";
    getline(cin, text);
    
    cout << "Text: \"" << text << "\"" << endl;
    cout << "Number of characters: " << text.length() << endl;
    
    // Create Huffman tree
    HuffmanTree huffman(text);
    
    // Print the tree
    huffman.printTree(true);  // true to show frequencies
    
    return 0;
}
