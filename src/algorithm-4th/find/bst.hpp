#ifndef _BST_TREE_H_
#define _BST_TREE_H_


#include <cstdio>

template<typename K, typename V>
class BSTree {
    struct Node {
        K key;           // key
        V val;         // associated data
        Node *left;
        Node *right;  // links to left and right subtrees
        Node(const K &k, const V &v) : key(k), val(v),
                                       left(nullptr),
                                       right(nullptr) {}
    };

    Node *root;     // root of the BST
    int count;

public:
    /**
     * Initializes an empty symbol table.
     */
    BSTree() : root(nullptr), count(0) {
    }

    ~BSTree() {
        destroy();
    }

    /**
     * Returns the number of key-value pairs in this symbol table.
     *
     * @return the number of key-value pairs in this symbol table
     */
    int size() const { return size(root); }

    /**
     * Is this symbol table empty?
     *
     * @return {@code true} if this symbol table is empty and {@code false} otherwise
     */
    bool empty() const { return size() == 0; }

    /**
     * Does this symbol table contain the given key?
     *
     * @param key the key
     * @return {@code true} if this symbol table contains {@code key} and
     * {@code false} otherwise
     * @throws IllegalArgumentException if {@code key} is {@code nullptr}
     */
    bool contains(const K &key) const {
        return getNode(root, key) != nullptr;
    }

    /**
     * Returns the value associated with the given key.
     *
     * @param key the key
     * @return the value associated with the given key if the key is in the symbol table
     * and {@code nullptr} if the key is not in the symbol table
     * @throws IllegalArgumentException if {@code key} is {@code nullptr}
     */
    const V *get(const K &key) const {
        Node *node = getNode(root, key);
        return node ? &(node->val) : nullptr;
    }

    /**
     * Inserts the specified key-value pair into the symbol table, overwriting the old
     * value with the new value if the symbol table already contains the specified key.
     * removes the specified key (and its associated value) from this symbol table
     * if the specified value is {@code nullptr}.
     *
     * @param key the key
     * @param val the value
     * @throws IllegalArgumentException if {@code key} is {@code nullptr}
     */
    void add(const K &key, const V &val) {
        if (root == nullptr) {
            root = new Node(key, val);
        } else {
            Node *par = nullptr;
            Node *cur = root;
            while (cur != nullptr) {
                par = cur;
                cur = (cur->val > val) ? cur->left : cur->right;
            }
            if (par->val > val)
                par->left = new Node(key, val);
            else
                par->right = new Node(key, val);
        }
        count++;

        // root = add(root, key, val);
        // assert (check());
    }

    /**
     * Removes the specified key and its associated value from this symbol table
     * (if the key is in this symbol table).
     *
     * @param key the key
     * @throws IllegalArgumentException if {@code key} is {@code nullptr}
     */
    void remove(const K &key) {
        if (getNode(root, key) == nullptr) return;
        root = remove(root, key);
        // assert (check());
    }


    /**
     * Returns the height of the BST (for debugging).
     *
     * @return the height of the BST (a 1-Node* tree has height 0)
     */
    int height() const {
        return height(root);
    }

    /**
     * Returns the smallest key in the symbol table.
     *
     * @return the smallest key in the symbol table
     * @throws NoSuchElementException if the symbol table is empty
     */
    const K *min() const {
        Node *node = min(root);
        return node ? &(node->key) : nullptr;
    }

    /**
     * Returns the largest key in the symbol table.
     *
     * @return the largest key in the symbol table
     * @throws NoSuchElementException if the symbol table is empty
     */
    const K *max() const {
        Node *node = max(root);
        return node ? &(node->key) : nullptr;
    }

    /**
     * Returns the largest key in the symbol table less than or equal to {@code key}.
     *
     * @param key the key
     * @return the largest key in the symbol table less than or equal to {@code key}
     * @throws NoSuchElementException   if there is no such key
     * @throws IllegalArgumentException if {@code key} is {@code nullptr}
     */
    const K *floor(const K &key) const {
        Node *node = floor(root, key);
        return node ? &(node->key) : nullptr;
    }

    /**
     * Returns the smallest key in the symbol table greater than or equal to {@code key}.
     *
     * @param key the key
     * @return the smallest key in the symbol table greater than or equal to {@code key}
     * @throws NoSuchElementException   if there is no such key
     * @throws IllegalArgumentException if {@code key} is {@code nullptr}
     */
    const K *ceiling(const K &key) const {
        Node *node = ceiling(root, key);
        return node ? &(node->key) : nullptr;
    }

    /**
     * Return the key in the symbol table of a given {@code rank}.
     * This key has the property that there are {@code rank} keys in
     * the symbol table that are smaller. In other words, this key is the
     * ({@code rank}+1)st smallest key in the symbol table.
     *
     * @param rank the order statistic
     * @return the key in the symbol table of given {@code rank}
     * @throws IllegalArgumentException unless {@code rank} is between 0 and
     *                                  <em>n</em>–1
     */
    const K *select(int rank) const {
        if (rank < 0 || rank >= size())
            return nullptr;
        return &(select(root, rank)->key);
    }

    /**
     * Return the number of keys in the symbol table strictly less than {@code key}.
     *
     * @param key the key
     * @return the number of keys in the symbol table strictly less than {@code key}
     * @throws IllegalArgumentException if {@code key} is {@code nullptr}
     */
    int rank(const K &key) const {
        return rank(root, key);
    }

    /**
     * Removes the smallest key and associated value from the symbol table.
     *
     * @throws NoSuchElementException if the symbol table is empty
     */
    void removeMin() {
        root = removeMin(root);
        // assert (check());
    }

    /**
     * Removes the largest key and associated value from the symbol table.
     *
     * @throws NoSuchElementException if the symbol table is empty
     */
    void removeMax() {
        root = removeMax(root);
        // assert (check());
    }

    void destroy() {
        destroy(root);
        count = 0;
    }

private:
    /***************************************************************************
     *  Standard BST search.
     ***************************************************************************/
    /***************************************************************************
     *  Red-black tree insertion.
     ***************************************************************************/
    // insert the key-value pair in the subtree rooted at h
    Node *add(Node *h, const K &key, const V &val) {
        if (h == nullptr) {
            count++;
            return new Node(key, val);
        }
        if (key < h->key) h->left = add(h->left, key, val);
        else if (key > h->key) h->right = add(h->right, key, val);
        else h->val = val;

        return h;
    }

    /***************************************************************************
     *  BST tree deletion.
     ***************************************************************************/
    // remove the key-value pair with the minimum key rooted at h
    Node *removeMin(Node *h) {
        if (h == nullptr)
            return nullptr;
        if (h->left == nullptr) {
            Node *rightNode = h->right;
            delete h;
            count--;
            return rightNode;
        }
        h->left = removeMin(h->left);
        return h;
    }

    // remove the key-value pair with the maximum key rooted at h
    Node *removeMax(Node *h) {
        if (h == nullptr)
            return nullptr;
        if (h->right == nullptr) {
            Node *leftNode = h->left;
            delete h;
            count--;
            return leftNode;
        }
        h->right = removeMax(h->right);
        return h;
    }

    // remove the key-value pair with the given key rooted at h
    Node *remove(Node *node, const K &key) {
        if (node == nullptr)
            return nullptr;
        if (node->key > key) {
            node->left = remove(node->left, key);
        } else if (node->key < key) {
            node->right = remove(node->right, key);
        } else if (node->key == key) {
            if (node->left == nullptr) {
                Node *rightNode = node->right;
                delete node;
                count--;
                return rightNode;
            } else {
                Node *successor = min(node->right);
                node->key = successor->key;
                node->val = successor->val;
                node->right = remove(node->right, successor->key);
            }
        }
        return node;
    }


    /***************************************************************************
     *  BST tree usually functions.
     ***************************************************************************/
    int height(Node *x) const {
        if (x == nullptr) return 0;
        return 1 + max(height(x->left), height(x->right));
    }

    // number of Node* in subtree rooted at x; 0 if x is nullptr
    int size(Node *x) const {
        return x ? size(x->right) + size(x->left) + 1 : 0;
    }

    // value associated with the given key in subtree rooted at x; nullptr if no such key
    Node *getNode(Node *x, const K &key) const {
        while (x != nullptr) {
            if (key < x->key) x = x->left;
            else if (key > x->key) x = x->right;
            else return x;
        }
        return nullptr;
    }

    // the smallest key in subtree rooted at x; nullptr if no such key
    Node *min(Node *x) const {
        // assert x != nullptr;
        if (x == nullptr || x->left == nullptr) return x;
        else return min(x->left);
    }

    // the largest key in the subtree rooted at x; nullptr if no such key
    Node *max(Node *x) const {
        if (x == nullptr || x->right == nullptr) return x;
        else return max(x->right);
    }

    // the largest key in the subtree rooted at x less than or equal to the given key
    Node *floor(Node *x, const K &key) const {
        if (x == nullptr) return nullptr;
        if (key == x->key) return x;
        if (key < x->key) return floor(x->left, key);
        Node *t = floor(x->right, key);
        if (t != nullptr) return t;
        else return x;
    }

    // the smallest key in the subtree rooted at x greater than or equal to the given key
    Node *ceiling(Node *x, const K &key) const {
        if (x == nullptr) return nullptr;
        if (key == x->key) return x;
        if (key > x->key) return ceiling(x->right, key);
        Node *t = ceiling(x->left, key);
        if (t != nullptr) return t;
        else return x;
    }

    // Return key in BST rooted at x of given rank.
    // Precondition: rank is in legal range.
    Node *select(Node *x, int rank) const {
        if (x == nullptr) return nullptr;
        int leftSize = size(x->left);
        if (leftSize > rank) return select(x->left, rank);
        else if (leftSize < rank) return select(x->right, rank - leftSize - 1);
        else return x;
    }

    // number of keys less than key in the subtree rooted at x
    int rank(Node *x, const K &key) const {
        if (x == nullptr) return 0;
        if (key < x->key) return rank(x->left, key);
        else if (key > x->key) return 1 + size(x->left) + rank(x->right, key);
        else return size(x->left);
    }

    void destroy(Node *node) {
        if (node == nullptr)
            return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    int max(const int &a, const int &b) const {
        return a > b ? a : b;
    }
    /***************************************************************************
     *  Check integrity of red-black tree data structure.
     ***************************************************************************/
public:
    bool check() {
        bool bst = isBST();
        bool sizeConsistent = isSizeConsistent();
        bool RankConsistent = isRankConsistent();

        if (!bst) printf("Not in symmetric order\n");
        if (!sizeConsistent) printf("Subtree counts not consistent\n");
        if (!RankConsistent) printf("Ranks not consistent\n");
        return bst && sizeConsistent && RankConsistent;
    }

private:
    // does this binary tree satisfy symmetric order?
    // Note: this test also ensures that data structure is a binary tree since order is strict
    bool isBST() const {
        return isBST(root, min(root), max(root));
    }

    // is the tree rooted at x a BST with all keys strictly between min and max
    // (if min or max is nullptr, treat as empty constraint)
    bool isBST(Node *x, Node *min, Node *max) const {
        if (x == nullptr) return true;
        if (min != nullptr && x->key < min->key) return false;
        if (max != nullptr && x->key > max->key) return false;
        return isBST(x->left, min, x) && isBST(x->right, x, max);
    }

    // are the size fields correct?
    bool isSizeConsistent() const {
        return true;
    }


    // check that ranks are consistent
    bool isRankConsistent() const {
        for (int i = 0; i < size(); i++)
            if (i != rank(*select(i))) return false;
        return true;
    }
};

#endif
