#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include <cstdio>

template<typename K, typename V>
class AVLTree {
    struct Node {
        int size;
        int height;
        K key;
        V val;
        Node *left;
        Node *right;

        Node(K key, V value) : key(key), val(value), height(1), left(nullptr), right(nullptr) {}
    };

    Node *root;
    int count;
public:
    AVLTree() : root(nullptr), count(0) {}

    ~AVLTree() { destroy(); }

    /**
     * Returns the number of key-value pairs in this symbol table.
     *
     * @return the number of key-value pairs in this symbol table
     */
    int size() const { return count; }

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
    bool contains(const K &key) const { return getNode(root, key) != nullptr; }

    /**
     * Returns the value associated with the given key.
     *
     * @param key the key
     * @return the value associated with the given key if the key is in the symbol table
     * and {@code nullptr} if the key is not in the symbol table
     * @throws IllegalArgumentException if {@code key} is {@code nullptr}
     */
    const V *get(K key) const {
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
    void add(const K &key, const V &value) { root = add(root, key, value); }


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
    }

    /**
     * Removes the largest key and associated value from the symbol table.
     *
     * @throws NoSuchElementException if the symbol table is empty
     */
    void removeMax() {
        root = removeMax(root);
    }

    void destroy() {
        destroy(root);
        count = 0;
    }

private:
    /***************************************************************************
      *  Standard BST search.
    ***************************************************************************/
    // insert the key-value pair in the subtree rooted at h
    Node *add(Node *node, const K &key, const V &value) {
        if (node == nullptr) {
            count++;
            return new Node(key, value);
        }
        if (node->key < key) {
            node->right = add(node->right, key, value);
        } else if (node->key > key) {
            node->left = add(node->left, key, value);
        } else {
            node->val = value;
        }
        return keepBalance(node);
    }

    /***************************************************************************
     *  AVL tree deletion.
     ***************************************************************************/
    // remove the key-value pair with the minimum key rooted at h
    Node *removeMin(Node *node) {
        if (node->left == nullptr) {
            Node *rightNode = node->right;
            delete node;
            count--;
            return rightNode;
        }
        node->left = removeMin(node->left);
        return keepBalance(node);
    }

    Node *removeMax(Node *node) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->right == nullptr) {
            Node *leftNode = node->left;
            delete node;
            count--;
            return leftNode;
        }
        node->right = removeMax(node->right);
        return keepBalance(node);
    }

    Node *remove(Node *node, const K &key) {
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
        return keepBalance(node);
    }

    /***************************************************************************
     *  AVL tree helper functions.
     ***************************************************************************/
    int getBalanceFactor(const Node *node) {
        return node == nullptr ? 0 : height(node->left) - height(node->right);
    }

    Node *keepBalance(Node *node) {
        if (node == nullptr)
            return nullptr;
        //先更新高度
        node->height = max(height(node->left), height(node->right)) + 1;
        // LL 情况
        //        y                              x
        //       / \                           /   \
        //      x   T4     向右旋转 (y)        z     y
        //     / \       - - - - - - - ->    / \   / \
        //    z   T3                       T1  T2 T3 T4
        //   / \
        // T1   T2
        if (getBalanceFactor(node) > 1 && getBalanceFactor(node->left) >= 0) {
            return rightRotate(node);
        }
        // RR 情况
        //    y                             x
        //  /  \                          /   \
        // T1   x      向左旋转 (y)       y     z
        //     / \   - - - - - - - ->   / \   / \
        //   T2  z                     T1 T2 T3 T4
        //      / \
        //     T3 T4
        if (getBalanceFactor(node) < -1 && getBalanceFactor(node->right) <= 0) {
            return leftRotate(node);
        }
        // LR 情况
        //        y                     y
        //       / \                   / \                     z
        //      x   T4    左旋x       z   T4   右旋y         /   \
        //     / \       - - - >     / \      - - - >       x     y
        //    T3  z                 x   T2                 / \   / \
        //       / \               / \                   T3  T1 T2  T4
        //      T1   T2           T3 T1
        if (getBalanceFactor(node) > 1 && getBalanceFactor(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // RL 情况
        //   y                         y
        //  /  \                      / \                        z
        // T1   x        右旋x       T1  z        左旋y         /   \
        //     / \      - - - >         / \      - - - >       y     x
        //    z  T2                    T3  x                  / \   / \
        //   / \                          / \               T1  T3 T4  T2
        //  T3 T4                        T4 T2
        if (getBalanceFactor(node) < -1 && getBalanceFactor(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    //RR
    // 对节点y进行向左旋转操作，返回旋转后新的根节点x
    //    y                             x
    //  /  \                          /   \
    // T1   x      向左旋转 (y)       y     z
    //     / \   - - - - - - - ->   / \   / \
    //   T2  z                     T1 T2 T3 T4
    //      / \
    //     T3 T4

    Node *leftRotate(Node *y) {
        Node *x = y->right;
        Node *T2 = x->left;

        //左旋转
        x->left = y;
        y->right = T2;

        //更新 x y 的值，因为y 在下面，所以先更新 y 的高度值，后更新x的高度值
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    //LL
    // 对节点y进行向右旋转操作，返回旋转后新的根节点x
    //        y                              x
    //       / \                           /   \
    //      x   T4     向右旋转 (y)        z     y
    //     / \       - - - - - - - ->    / \   / \
    //    z   T3                       T1  T2 T3 T4
    //   / \
    // T1   T2
    // x->right = y
    // y->left = T3
    Node *rightRotate(Node *y) {
        Node *x = y->left;
        Node *T3 = x->right;
        //右旋转
        x->right = y;
        y->left = T3;
        //更新 x y 的值，因为y 在下面，所以先更新 y 的高度值，后更新x的高度值
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    /***************************************************************************
     *  BST tree usually functions.
     ***************************************************************************/
    int height(const Node *node) const {
        return node == nullptr ? 0 : node->height;
    }

    int size(Node *x) const {
        if (!x) return 0;
        return 1 + size(x->left) + size(x->right);
    }

    // value associated with the given key in subtree rooted at x; nullptr if no such key
    Node *getNode(Node *node, const K &key) const {
        if (!node || node->key == key)
            return node;
        if (key < node->key)
            return getNode(node->left, key);
        else //if (key > node->key)
            return getNode(node->right, key);
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

//    // Return key in BST rooted at x of given rank.
//    // Precondition: rank is in legal range.
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
        bool balanced = isBalanced();

        if (!bst) printf("Not in symmetric order\n");
        if (!sizeConsistent) printf("Subtree counts not consistent\n");
        if (!RankConsistent) printf("Ranks not consistent\n");
        if (!balanced) printf("Not balanced\n");
        return bst && sizeConsistent && RankConsistent && balanced;
    }

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
        return isSizeConsistent(root);
    }

    bool isSizeConsistent(Node *x) const {
        if (x == nullptr) return true;
        if (size(x) != size(x->left) + size(x->right) + 1) return false;
        return isSizeConsistent(x->left) && isSizeConsistent(x->right);
    }

    // check that ranks are consistent
    bool isRankConsistent() const {
        for (int i = 0; i < size(); i++)
            if (i != rank(*select(i))) return false;
        return true;
    }

    bool isBalanced() const {
        return isBalanced(root) >= 0;
    }

    int isBalanced(Node *node) const {
        if (node == nullptr)
            return 0;
        int leftHight = isBalanced(node->left);
        if (leftHight < 0)
            return leftHight;
        int rightHight = isBalanced(node->right);
        if (rightHight < 0)
            return rightHight;
        if (leftHight - rightHight < 2 || rightHight - leftHight < 2)
            return max(leftHight, rightHight) + 1;
        else
            return -1;
    }
};

#endif

