#ifndef _RBT_TREE_H_
#define _RBT_TREE_H_

template<typename K, typename V>
class RBTree {
    static const bool RED = true;
    static const bool BLACK = false;

    struct Node {
        K key;           // key
        V val;         // associated data
        Node *left;
        Node *right;  // links to left and right subtrees
        bool color;     // color of parent link
        int size;          // subtree count

        Node(const K &k, const V &v, bool color, int size) : key(k), val(v), color(color), size(size),
                                                             left(nullptr),
                                                             right(nullptr) {}
    };

    Node *root;     // root of the BST
public:
    /**
     * Initializes an empty symbol table.
     */
    RBTree() : root(nullptr) {
    }

    ~RBTree() { destroy(); }

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
        root = add(root, key, val);
        root->color = BLACK;
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
        // if both children of root are black, set root to red
        if (!isRed(root->left) && !isRed(root->right))
            root->color = RED;

        root = remove(root, key);
        if (!empty()) root->color = BLACK;
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
        // if both children of root are black, set root to red
        if (!isRed(root->left) && !isRed(root->right))
            root->color = RED;

        root = removeMin(root);
        if (!empty()) root->color = BLACK;
        // assert (check());
    }

    /**
     * Removes the largest key and associated value from the symbol table.
     *
     * @throws NoSuchElementException if the symbol table is empty
     */
    void removeMax() {
        // if both children of root are black, set root to red
        if (!isRed(root->left) && !isRed(root->right))
            root->color = RED;

        root = removeMax(root);
        if (!empty()) root->color = BLACK;
        // assert (check());
    }

    void destroy() {
        destroy(root);
    }

private:
    /***************************************************************************
     *  Standard BST search.
     ***************************************************************************/
    // insert the key-value pair in the subtree rooted at h
    Node *add(Node *h, const K &key, const V &val) {
        if (h == nullptr) return new Node(key, val, RED, 1);

        if (key < h->key) h->left = add(h->left, key, val);
        else if (key > h->key) h->right = add(h->right, key, val);
        else h->val = val;

        // fix-up any right-leaning links
        if (isRed(h->right) && !isRed(h->left)) h = rotateLeft(h);
        if (isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);
        if (isRed(h->left) && isRed(h->right)) flipColors(h);
        h->size = size(h->left) + size(h->right) + 1;

        return h;
    }

    /***************************************************************************
     *  Red-black tree deletion.
     ***************************************************************************/
    // remove the key-value pair with the minimum key rooted at h
    Node *removeMin(Node *h) {
        if (h->left == nullptr) {
            delete h;
            return nullptr;
        }

        if (!isRed(h->left) && !isRed(h->left->left))
            h = moveRedLeft(h);

        h->left = removeMin(h->left);
        return balance(h);
    }

    // remove the key-value pair with the maximum key rooted at h
    Node *removeMax(Node *h) {
        if (isRed(h->left))
            h = rotateRight(h);

        if (h->right == nullptr) {
            delete h;
            return nullptr;
        }

        if (!isRed(h->right) && !isRed(h->right->left))
            h = moveRedRight(h);

        h->right = removeMax(h->right);
        return balance(h);
    }

    // remove the key-value pair with the given key rooted at h
    Node *remove(Node *h, const K &key) {
        if (key < h->key) {
            if (!isRed(h->left) && !isRed(h->left->left))
                h = moveRedLeft(h);
            h->left = remove(h->left, key);
        } else {
            if (isRed(h->left))
                h = rotateRight(h);
            if (key == h->key && (h->right == nullptr)) {
                delete h;
                return nullptr;
            }
            if (!isRed(h->right) && !isRed(h->right->left))
                h = moveRedRight(h);
            if (key == h->key) {
                Node *x = min(h->right);
                h->key = x->key;
                h->val = x->val;
                // h->val = get(h->right, min(h->right)->key);
                // h->key = min(h->right)->key;
                h->right = removeMin(h->right);
            } else h->right = remove(h->right, key);
        }
        return balance(h);
    }

    /***************************************************************************
     *  Red-black tree helper functions.
     ***************************************************************************/
    // make a left-leaning link lean to the right
    //     node                   x
    //    /   \     右旋转       /  \
    //   x    T2   ------->   y   node
    //  / \                       /  \
    // y  T1                     T1  T2

    Node *rotateRight(Node *node) {
//        assert((h != nullptr) && isRed(h->left));
//        assert (h != nullptr) && isRed(h->left) &&  !isRed(h->right);  // for insertion only
        Node *x = node->left;
        node->left = x->right;
        x->right = node;
        x->color = x->right->color;
        x->right->color = RED;

        x->size = node->size;
        node->size = size(node->left) + size(node->right) + 1;
        return x;
    }

    // make a right-leaning link lean to the left
    //   node                     x
    //  /   \     左旋转         /  \
    // T1    x   --------->   node   T3
    //      / \              /   \
    //     T2 T3            T1   T2

    Node *rotateLeft(Node *node) {
//        assert((h != nullptr) && isRed(h->right));
//        assert ((h != nullptr) && isRed(h->right) && !isRed(h->left));  // for insertion only
        Node *x = node->right;
        node->right = x->left;
        x->left = node;
        x->color = x->left->color;
        x->left->color = RED;

        x->size = node->size;
        node->size = size(node->left) + size(node->right) + 1;
        return x;
    }

    // flip the colors of a Node* and its two children
    void flipColors(Node *h) {
        // h must have opposite color of its two children
        // assert ((h != nullptr) && (h->left != nullptr) && (h->right != nullptr));
        // assert ((!isRed(h) &&  isRed(h->left) &&  isRed(h->right))
        //    || (isRed(h)  && !isRed(h->left) && !isRed(h->right)));
        h->color = !h->color;
        h->left->color = !h->left->color;
        h->right->color = !h->right->color;
    }

    // Assuming that h is red and both h->left and h->left->left
    // are black, make h->left or one of its children red.
    Node *moveRedLeft(Node *h) {
        // assert (h != nullptr);
        // assert (isRed(h) && !isRed(h->left) && !isRed(h->left->left));

        flipColors(h);
        if (isRed(h->right->left)) {
            h->right = rotateRight(h->right);
            h = rotateLeft(h);
            flipColors(h);
        }
        return h;
    }

    // Assuming that h is red and both h->right and h->right->left
    // are black, make h->right or one of its children red.
    Node *moveRedRight(Node *h) {
        // assert (h != nullptr);
        // assert (isRed(h) && !isRed(h->right) && !isRed(h->right->left));
        flipColors(h);
        if (isRed(h->left->left)) {
            h = rotateRight(h);
            flipColors(h);
        }
        return h;
    }

    // restore red-black tree invariant
    Node *balance(Node *h) {
        // assert (h != nullptr);
        if (isRed(h->right) && !isRed(h->left)) h = rotateLeft(h);
        if (isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);
        if (isRed(h->left) && isRed(h->right)) flipColors(h);

        h->size = size(h->left) + size(h->right) + 1;
        return h;
    }

    // is Node* x red; false if x is nullptr ?
    bool isRed(Node *x) const {
        return x ? x->color == RED : false;
    }

    /***************************************************************************
     *  BST tree usually functions.
     ***************************************************************************/
    int height(Node *x) const {
        if (x == nullptr) return 0;
        return 1 + std::max(height(x->left), height(x->right));
    }

    // number of Node* in subtree rooted at x; 0 if x is nullptr
    int size(Node *x) const {
        return x ? x->size : 0;
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

    int max(const int &a, const int &b) {
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
        bool rbNode = is23();
        bool balanced = isBalanced();

        if (!bst) printf("Not in symmetric order\n");
        if (!sizeConsistent) printf("Subtree counts not consistent\n");
        if (!RankConsistent) printf("Ranks not consistent\n");
        if (!rbNode) printf("Not a 2-3 tree\n");
        if (!balanced) printf("Not balanced\n");
        return bst && sizeConsistent && RankConsistent && rbNode && balanced;
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
        return isSizeConsistent(root);
    }

    bool isSizeConsistent(Node *x) const {
        if (x == nullptr) return true;
        if (x->size != size(x->left) + size(x->right) + 1) return false;
        return isSizeConsistent(x->left) && isSizeConsistent(x->right);
    }

    // check that ranks are consistent
    bool isRankConsistent() const {
        for (int i = 0; i < size(); i++)
            if (i != rank(*select(i))) return false;
        return true;
    }

    // Does the tree have no red right links, and at most one (left)
    // red links in a row on any path?
    bool is23() const {
        return is23(root);
    }

    bool is23(Node *x) const {
        if (x == nullptr) return true;
        if (isRed(x->right)) return false;
        if (x != root && isRed(x) && isRed(x->left))
            return false;
        return is23(x->left) && is23(x->right);
    }

    // do all paths from root to leaf have same number of black edges?
    bool isBalanced() const {
        int black = 0;     // number of black links on path from root to min
        Node *x = root;
        while (x != nullptr) {
            if (!isRed(x)) black++;
            x = x->left;
        }
        return isBalanced(root, black);
    }

    // does every path from the root to a leaf have the given number of black links?
    bool isBalanced(Node *x, int black) const {
        if (x == nullptr) return black == 0;
        if (!isRed(x)) black--;
        return isBalanced(x->left, black) && isBalanced(x->right, black);
    }
};

#endif