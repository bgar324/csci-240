#include <iostream>
#include <vector>
#include <string>
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

//helper functions

void preorder(LinkedBinaryTree<string>::Position p, vector<LinkedBinaryTree<string>::Position>& vec){
  if(p.is_null()){
    return;
  }
  vec.push_back(p);
  if(!p.left().is_null()){
    preorder(p.left(), vec);
  }
  if(!p.right().is_null()){
    preorder(p.right(), vec);
  }
}

vector<LinkedBinaryTree<string>::Position> preorder_positions(const LinkedBinaryTree<string>& tree) {
  vector<LinkedBinaryTree<string>::Position> result;
  if (!tree.empty())
      preorder(tree.root(), result);
  return result;
}

int main() {
  // ------- Test Case 1 -------
  cout << "Test Case 1 (Expected: A B D C E F):" << endl;

  LinkedBinaryTree<string> tree;
  tree.add_root("A");

  auto root = tree.root();
  tree.add_left(root, "B");
  tree.add_right(root, "C");

  auto B = root.left();
  auto C = root.right();

  tree.add_left(B, "D");
  tree.add_left(C, "E");
  tree.add_right(C, "F");

  auto positions1 = preorder_positions(tree);
  for (auto pos : positions1)
      cout << pos.element() << " ";
  cout << endl;

  // ------- Test Case 2 -------
  // Remove B (which has only one child D)
  tree.erase(B);

  cout << "Test Case 2 (Expected: A D C E F):" << endl;
  auto positions2 = preorder_positions(tree);
  for (auto pos : positions2)
      cout << pos.element() << " ";
  cout << endl;

  return 0;
}