#include <iostream>
#include <vector>

using namespace std;

template <typename E>
class LinkedBinaryTree
{
protected:
    //------ nested Node class ------
    class Node
    {
    public: // members public for convenience, as Node class is protected
        E element;
        Node *parent;
        Node *left{nullptr};
        Node *right{nullptr};

        Node(E e, Node *p = nullptr) : element{e}, parent{p} {}
    }; // end of Node class

    //------ data members of LinkedBinaryTree ------
    Node *rt{nullptr};
    int sz{0};

public:
    //------ nested Position class ------
    class Position
    {
    private:
        Node *node;
        friend class LinkedBinaryTree; // allow outer class access to node pointer

    public:
        Position(Node *nd = nullptr) : node{nd} {}

        bool operator==(Position other) const { return node == other.node; }
        bool operator!=(Position other) const { return node != other.node; }

        bool is_null() const { return node == nullptr; }
        bool is_root() const { return node->parent == nullptr; }
        bool is_external() const { return node->left == nullptr && node->right == nullptr; }

        E &element() { return node->element; } // will allow element to be edited
        const E &element() const { return node->element; }
        Position parent() const { return Position(node->parent); }
        Position left() const { return Position(node->left); }
        Position right() const { return Position(node->right); }

        vector<Position> children() const
        {
            vector<Position> result;
            if (node->left != nullptr)
                result.push_back(Position(node->left));
            if (node->right != nullptr)
                result.push_back(Position(node->right));
            return result;
        }

        int num_children() const
        {
            int result{0};
            if (node->left != nullptr)
                result++;
            if (node->right != nullptr)
                result++;
            return result;
        }
    }; // end of Position class

    //------ member functions of LinkedBinaryTree ------

    LinkedBinaryTree() {} // Constructs a tree with zero nodes

    int size() const { return sz; }        // Returns the number of elements in the tree
    bool empty() const { return sz == 0; } // True if the tree has size 0

    Position root() const { return Position(rt); } // Returns a Position for the root (possibly null)

    vector<Position> positions() const
    { // Returns an inorder sequence of positions
        vector<Position> gather;
        if (rt)
            inorder(Position(rt), gather); // (the inorder function is defined in later section)
        return gather;
    }

    void add_root(const E &e = E())
    { // add root to (presumed) empty tree
        rt = new Node(e);
        sz = 1;
    }

    void add_left(Position p, const E &e)
    {                                       // Creates a new left child of p storing e
        p.node->left = new Node{e, p.node}; // parent of new node is p's node
        sz++;
    }

    void add_right(Position p, const E &e)
    {                                        // Creates a new left child of p storing e
        p.node->right = new Node{e, p.node}; // parent of new node is p's node
        sz++;
    }

    void erase(Position p)
    { // Removes p, promoting its one child, if any
        Node *nd = p.node;
        Node *child{nd->left == nullptr ? nd->right : nd->left}; // lone child or else nullptr

        if (child != nullptr)
            child->parent = nd->parent; // child's grandparent becomes parent

        if (nd == rt)
            rt = child;                  // child promoted to root
        else if (nd->parent->left == nd) // node is left child of its parent
            nd->parent->left = child;
        else // node is right child of its parent
            nd->parent->right = child;
        sz--;
        delete nd;
    }

    // Attaches the internal structures of trees left and right and subtrees of leaf p.
    void attach(Position p, LinkedBinaryTree &left, LinkedBinaryTree &right)
    {
        Node *nd = p.node;
        nd->left = left.rt;
        nd->right = right.rt;
        sz += left.sz + right.sz;

        // adjust parent pointers for old roots (if they exist)
        if (left.rt)
            left.rt->parent = nd;
        if (right.rt)
            right.rt->parent = nd;

        // reset other trees to an empty state
        left.sz = right.sz = 0;
        left.rt = right.rt = nullptr;
    }

    // ------------- Rule-of-five support ----------------
private:
    void tear_down(Node *nd)
    {
        if (nd != nullptr)
        {
            tear_down(nd->left);
            tear_down(nd->right);
            delete nd;
        }
    }

    // Create cloned structure of model and return pointer to the new structure
    static Node *clone(Node *model)
    {
        if (model == nullptr)
            return nullptr; // trivial clone
        Node *new_root = new Node(model->element);
        new_root->left = clone(model->left);
        if (new_root->left)
            new_root->left->parent = new_root;
        new_root->right = clone(model->right);
        if (new_root->right)
            new_root->right->parent = new_root;
        return new_root;
    }

public:
    // destructor
    ~LinkedBinaryTree() { tear_down(rt); }

    // copy constructor
    LinkedBinaryTree(const LinkedBinaryTree &other) : sz{other.sz}, rt{clone(other.rt)} {}

    // copy assignment
    LinkedBinaryTree &operator=(const LinkedBinaryTree &other)
    {
        if (this != &other)
        { // bypass self-assignment
            tear_down(rt);
            rt = clone(other.rt);
            sz = other.sz;
        }
        return *this;
    }

    // move constructor
    LinkedBinaryTree(LinkedBinaryTree &&other) : sz{other.sz}, rt{other.rt}
    {
        // reset other to empty
        other.sz = 0;
        other.rt = nullptr;
    }

    // move assignment
    LinkedBinaryTree &operator=(LinkedBinaryTree &&other)
    {
        if (this != &other)
        { // bypass self-assignment
            using std::swap;
            swap(sz, other.sz);
            swap(rt, other.rt); // old structure will be destroyed by other
        }
        return *this;
    }
}; // end of LinkedBinaryTree class

template <typename Key, typename Value>
class AbstractMap
{
public:
    //-------- nested Entry class ---------
    class Entry
    {
        friend AbstractMap;

    private:
        Key k;
        Value v;

    public:
        Entry(const Key &k = Key(), const Value &v = Value()) : k(k), v(v) {}
        const Key &key() const { return k; }     // read-only access
        const Value &value() const { return v; } // read-only access
        Value &value() { return v; }             // allow change to value
    }; // end of Entry class

protected:
    //---------- custom iterator representation ----------
    class abstract_iter_rep
    {
    public:
        virtual const Entry &entry() const = 0;
        virtual void advance() = 0;
        virtual bool equals(const abstract_iter_rep *other) const = 0;
        virtual abstract_iter_rep *clone() const = 0;
        virtual ~abstract_iter_rep() {}
    }; //------ end of abstract_iter_rep ----------

public:
    //---------- const_iterator ----------
    class const_iterator
    {
        friend AbstractMap;

    private:
        abstract_iter_rep *rep{nullptr}; // a pointer to an underlying iterator representation

    public:
        const Entry &operator*() const { return rep->entry(); }
        const Entry *operator->() const { return &rep->entry(); }
        const_iterator &operator++()
        {
            rep->advance();
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator temp{*this};
            rep->advance();
            return temp;
        }
        bool operator==(const const_iterator &other) const { return rep->equals(other.rep); }
        bool operator!=(const const_iterator &other) const { return !rep->equals(other.rep); }

        const_iterator(abstract_iter_rep *r = nullptr) : rep{r} {}
        const_iterator(const const_iterator &other) : rep{other.rep->clone()} {}
        ~const_iterator() { delete rep; }
        const_iterator &operator=(const const_iterator &other)
        {
            if (this != &other and rep != nullptr)
            {
                delete rep;
                rep = other.rep->clone();
            }
            return *this;
        }
    }; //------ end of const_iterator ----------

protected:
    // necessary utilities for our inheritance
    abstract_iter_rep *get_rep(const_iterator iter) const { return iter.rep; }
    void update_value(const Entry &e, const Value &v) { const_cast<Entry &>(e).v = v; }

public:
    //---------- pure virtual functions -----------
    virtual int size() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator find(const Key &k) const = 0;
    virtual const_iterator put(const Key &k, const Value &v) = 0;
    virtual const_iterator erase(const_iterator loc) = 0;

    //---------- concrete functions -----------
    bool empty() const { return size() == 0; }                     // Returns true if the map is empty, false otherwise
    bool contains(const Key &k) const { return find(k) != end(); } // Returns true if map contains key

    // Returns the value associated with key k,  or throws out_of_range exception if k not found
    const Value &at(const Key &k) const
    {
        const_iterator it{find(k)};
        if (it == end())
            throw std::out_of_range("key not found");
        return it->value();
    }

    // Erases entry with given key (if one exists); returns true if entry was removed
    bool erase(const Key &k)
    {
        const_iterator it{find(k)};
        if (it == end())
            return false;
        erase(it);
        return true;
    }

    virtual ~AbstractMap() {} // expected when declaring other virtual functions
};
template <typename Key, typename Value, typename Compare = less<Key>>
class TreeMap : public AbstractMap<Key, Value>
{
protected:
    // ------------ nested BalanceableBinaryTree class -------------------
    typedef pair<typename TreeMap::Entry, int> BSTEntry; // include auxiliary int for balancing info
    typedef LinkedBinaryTree<BSTEntry> TreeBase;
    class BalanceableBinaryTree : public TreeBase
    {
    public:
        friend TreeMap;
        using TreeBase::rt, typename TreeBase::Node;

        Node *sentinel() const { return TreeBase::rt; }

        // link child (possibly nullptr) to the indicated side of the parent
        void relink(Node *parent, Node *child, bool make_left_child)
        {
            if (make_left_child)
                parent->left = child;
            else
                parent->right = child;
            if (child != nullptr)
                child->parent = parent;
        }

        // Rotates non-root node x above its parent.
        void rotate(Node *x)
        {
            Node *y = x->parent;        // we assume parent exists within the tree map
            Node *z = y->parent;        // grandparent (possibly the sentinel)
            relink(z, x, y == z->left); // x becomes direct child of z
            // now rotate x and y, including transfer of middle subtree
            if (x == y->left)
            {
                relink(y, x->right, true); // x's right child becomes y's left child
                relink(x, y, false);       // y becomes x's right child
            }
            else
            {
                relink(y, x->left, false); // x's left child becomes y's right child
                relink(x, y, true);        // y becomes left child of x
            }
        }

        // Performs a trinode restructuring of Node x with its parent/grandparent.
        // Returns the Node that becomes the root of the restructured subtree.
        Node *restructure(Node *x)
        {
            Node *y = x->parent;
            Node *z = y->parent;
            if ((x == y->right) == (y == z->right))
            {              // matching alignments
                rotate(y); // single rotation (of y)
                return y;  // y is new subtree root
            }
            else
            {              // opposite alignments
                rotate(x); // double rotation (of x)
                rotate(x);
                return x; // x is new subtree root
            }
        }
    }; // --------- end of nested BalanceableBinaryTree class ------------
protected:
    typedef AbstractMap<Key, Value> Base; // shorthand for the base class

public:
    using Base::empty, Base::erase, typename Base::Entry, typename Base::const_iterator;

protected:
    // instance variables for a TreeMap
    Compare less_than;          // determines "a < b" relationship among keys
    BalanceableBinaryTree tree; // nested BalanceableBinaryTree class shown separately
    typedef typename BalanceableBinaryTree::Node Node;
    typedef typename BalanceableBinaryTree::Position Position;
    int aux(Node *p) const
    {
        return p->element.second;
    }

    void set_aux(Node *p, int h)
    {
        p->element.second = h;
    }

    Key key(Node *nd) const { return nd->element.first.key(); }

    bool equals(Key a, Key b) const
    { // equality based on the less_than comparator
        return (!less_than(a, b) && !less_than(b, a));
    }

    // Return pointer to node storing key k, or the last node on a failed search
    Node *search(Key k) const
    {
        if (empty())
            return tree.rt;
        Node *walk = tree.rt->left; // start to the left of end sentinel
        while (true)
        {
            if (less_than(k, key(walk)))
            { // consider left subtree
                if (walk->left == nullptr)
                    return walk;   //     failed search
                walk = walk->left; //     continue at left child
            }
            else if (less_than(key(walk), k))
            { // consider right subtree
                if (walk->right == nullptr)
                    return walk;    //     failed search
                walk = walk->right; //     continue at right child
            }
            else
            {
                return walk; // successful match for key
            }
        }
    }

    // return the inorder successor of position p
    static Node *successor(Node *p)
    {
        if (p->right == nullptr)
        { // no right subtree so look upward
            while (p == p->parent->right)
                p = p->parent;
            return p->parent; // original p was max of left subtree of returned
        }
        else
        {
            // find smallest entry in right subtree
            p = p->right;
            while (p->left != nullptr)
                p = p->left;
            return p;
        }
    }

    //------- nested iter_rep class for map ADT --------
    // a position within our map is described by a node pointer
    using Base::get_rep;
    using typename Base::abstract_iter_rep;
    class iter_rep : public abstract_iter_rep
    { // specialize abstract version
    public:
        Node *node;
        iter_rep(Node *nd) : node{nd} {}

        const Entry &entry() const { return node->element.first; }
        void advance() { node = successor(node); }
        abstract_iter_rep *clone() const { return new iter_rep(node); }
        bool equals(const abstract_iter_rep *other) const
        {
            const iter_rep *p = dynamic_cast<const iter_rep *>(other);
            return p != nullptr && node == p->node;
        }
    }; //------- end of class iter_rep --------

public:
    // Creates an empty map
    TreeMap()
    {
        tree.add_root(); // tree root serves as end() sentinel; entry is irrelevant
    }

    virtual void rebalance_insert(Node *p) {}
    virtual void rebalance_delete(Node *p) {}
    virtual void rebalance_access(Node *p) {}

    // Returns the number of entries in the map
    int size() const { return tree.size() - 1; } // disregard the end sentinel

    // Returns a const_iterator to first entry
    const_iterator begin() const
    {
        Node *walk = tree.sentinel();
        while (walk->left != nullptr)
            walk = walk->left;
        return const_iterator(new iter_rep(walk));
    }

    // Returns a const_iterator representing the end
    const_iterator end() const { return const_iterator(new iter_rep(tree.rt)); }

    // Returns a const_iterator to the entry with a given key, or end() if no such entry exists
    const_iterator find(const Key &k) const
    {
        if (empty())
            return end();
        Node *p{search(k)};
        const_cast<TreeMap *>(this)->rebalance_access(p);
        if (equals(k, key(p))) // exact match
            return const_iterator(new iter_rep(p));
        else // unsuccessful search
            return end();
    }

    // Associates given key with given value; if key already exists previous value is overwritten
    const_iterator put(const Key &k, const Value &v)
    {
        Node *p{search(k)};
        if (p != tree.rt && equals(k, key(p)))
        {                                 // exact match
            p->element.first.value() = v; // update entry's value
            rebalance_access(p);
        }
        else
        { // unsuccessful search
            if (p == tree.rt || less_than(k, key(p)))
            {
                tree.add_left(Position(p), {{k, v}, 0});
                p = p->left;
            }
            else
            {
                tree.add_right(Position(p), {{k, v}, 0});
                p = p->right;
            }
            rebalance_insert(p);
        }
        return const_iterator(new iter_rep(p));
    }

    // Removes entry referenced by given iterator; returns iterator to next entry in iteration order
    const_iterator erase(const_iterator loc)
    {
        Node *p = dynamic_cast<iter_rep *>(get_rep(loc))->node;
        if (p->left != nullptr && p->right != nullptr)
        { // p has two children
            Node *before = p->left;
            while (before->right != nullptr)
                before = before->right;
            p->element.first = before->element.first; // move predecessor's entry to p
            p = before;                               // and now consider deleting predecessor
        }
        // now p has at most one child
        Node *parent = p->parent;
        Node *after = successor(p);
        tree.erase(Position(p)); // inherited from LinkedBinaryTree
        rebalance_delete(parent);
        return const_iterator(new iter_rep(after));
    }

    // Returns an iterator to the first entry with key greater than or equal to k, or else end()
    const_iterator lower_bound(const Key &k) const
    {
        if (empty())
            return end();
        Node *p{search(k)};
        if (less_than(key(p), k)) // unsuccessful search ended at leaf with smaller key
            p = successor(p);
        return const_iterator(new iter_rep(p));
    }

    // Returns an iterator to the first entry with key strictly greater than k, or else end()
    const_iterator upper_bound(const Key &k) const
    {
        if (empty())
            return end();
        Node *p{search(k)};
        if (!less_than(k, key(p))) // need entry with strictly greater key
            p = successor(p);
        return const_iterator(new iter_rep(p));
    }
};

template <typename Key, typename Value, typename Compare = less<Key>>
class AVLTreeMap : public TreeMap<Key, Value, Compare>
{
protected:
    typedef TreeMap<Key, Value, Compare> Base;
    using Base::tree, Base::aux, Base::set_aux, typename Base::Node;

    // Returns the height of the given node (nullptr is considered 0)
    int height(Node *p) const
    {
        return (p == nullptr ? 0 : aux(p));
    }

    // Recomputes the height of the given position based on its subtree's heights
    void recompute_height(Node *p)
    {
        set_aux(p, 1 + max(height(p->left), height(p->right)));
    }

    // Returns true if position has balance factor between -1 and +1 inclusive
    bool is_balanced(Node *p) const
    {
        return abs(height(p->left) - height(p->right)) <= 1;
    }

    // Returns a child of p with height no smaller than that of the other child
    // In case of tie, chooses the "aligned" child
    Node *taller_child(Node *p)
    {
        if (height(p->left) > height(p->right))
            return p->left; // clear winner
        if (height(p->left) < height(p->right))
            return p->right; // clear winner
        // with equal height children, break tie while matching parent's orientation
        if (p == p->parent->left)
            return p->left;
        else
            return p->right;
    }

    // Utility used to rebalance after an insertion or deletion. This traverses the
    // path upward from p, performing a trinode restructuring when imbalance is found,
    // continuing until balance is restored.
    void rebalance(Node *p)
    {
        int old_height, new_height;
        do
        {
            old_height = height(p); // not yet recalculated if internal
            if (!is_balanced(p))
            { // imbalance detected
                // perform trinode restructuring, setting p to resulting root,
                // and recompute new local heights after the restructuring
                p = tree.restructure(taller_child(taller_child(p)));
                recompute_height(p->left);
                recompute_height(p->right);
            }
            recompute_height(p);
            new_height = height(p);
            p = p->parent;
        } while (old_height != new_height && p != tree.sentinel());
    }

    // Rebalances the tree immediately after inserting the given node
    void rebalance_insert(Node *p) { rebalance(p); }

    // Rebalances the the tree immediately after the deletion of a child of p
    void rebalance_delete(Node *p)
    {
        if (p != tree.sentinel())
            rebalance(p);
    }

public:
//access the node behind the iterator
    int getHeight(const typename AVLTreeMap::const_iterator &it) const
    {
        auto rep = dynamic_cast<typename Base::iter_rep *>(Base::get_rep(it)); 
        return height(rep->node);
    }
};

int main() {
    AVLTreeMap<int, string> tree;

    tree.put(20, "Alice");
    tree.put(4, "Bob");
    tree.put(26, "Cathy");
    tree.put(3, "Dan");
    tree.put(9, "Eva");
    tree.put(15, "Frank");
    tree.put(30, "Grace");

    cout << "\nSearching for 15: ";
    cout << (tree.find(15) != tree.end() ? "found" : "not found") << endl;

    cout << "Searching for 30: ";
    cout << (tree.find(30) != tree.end() ? "found" : "not found") << endl;

    cout << "Searching for 8: ";
    cout << (tree.find(8) != tree.end() ? "found" : "not found") << endl;

    cout << "\nAVL Tree (Inorder Traversal):\n";
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        cout << "Key: " << it->key()
             << ", Value: " << it->value()
             << ", Height: " << tree.getHeight(it)
             << endl;
    }

    return 0;
}
