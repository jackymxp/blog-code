#include<iostream>
#include<stack>
#include <algorithm>


template<typename T>
struct tree_node {
    typedef tree_node *pointer;
    pointer left;
    pointer right;
    T data;

    explicit tree_node(T t, pointer left = nullptr, pointer right = nullptr) : data(t), left(left), right(right) {
    }
};

template<class T, class Ref, class Ptr>
class tree_iterator : public std::iterator<std::forward_iterator_tag, T> {
public:
    typedef tree_iterator<T, T &, T *> iterator;
    typedef tree_iterator<T, const T &, const T *> const_iterator;
    typedef tree_iterator<T, Ref, Ptr> self;

    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;

    typedef tree_node<T> *link_type;
    typedef size_t size_type;

    link_type node;

    tree_iterator(link_type x);

    tree_iterator() {}

    tree_iterator(const iterator &x);

    bool operator==(const self &x) const { return node == x.node; }

    bool operator!=(const self &x) const { return node != x.node; }

    reference operator*() { return (*node).data; }

    pointer operator->() { return &(operator*()); }

    self &operator++();

private:
    std::stack<link_type> stack;
};

template<class T, class Ref, class Ptr>
tree_iterator<T, Ref, Ptr> &tree_iterator<T, Ref, Ptr>::operator++() {
    if (stack.empty()) {
        node = nullptr;
        return *this;
    }
    stack.pop();
    if (node->right != nullptr) {
        link_type temp = node->right;
        while (temp != nullptr) {
            stack.push(temp);
            temp = temp->left;
        }
    }
    if (stack.empty()) {
        node = nullptr;
    } else {
        node = stack.top();
    }
    // we should return an iterator pointing to next element;
    return *this;
}

template<class T, class Ref, class Ptr>
tree_iterator<T, Ref, Ptr>::tree_iterator(link_type x) {
    if (x == nullptr) {
        node = nullptr;
        return;
    }
    stack.push(x);
    node = x;
    while (x->left != nullptr) {
        stack.push(x->left);
        x = x->left;
        node = x;
    }
}

template<class T, class Ref, class Ptr>
tree_iterator<T, Ref, Ptr>::tree_iterator(const iterator &x) {
    node = x.node;
    stack = x.stack;
}

template<class T>
class BinaryTree {
public:
    typedef tree_iterator<T, T &, T *> iterator;
    //typedef __tree_iterator<T,const T&,const T*> const_iterator;
    typedef typename tree_iterator<T, T &, T *>::link_type link_type;

protected:
    link_type root;

public:
    BinaryTree();

    BinaryTree(link_type node);
    //void insert(T t);

    // this function will call __tree__iterator' constructor function
    iterator begin();//{return root;}
    iterator end();//{return iterator(nullptr);}
};

template<class T>
BinaryTree<T>::BinaryTree() {
    root = nullptr;
}


template<class T>
BinaryTree<T>::BinaryTree(link_type node) {
    root = node;
}

template<class T>
typename BinaryTree<T>::iterator BinaryTree<T>::begin() {
    return root;
}

template<class T>
typename BinaryTree<T>::iterator BinaryTree<T>::end() {
    return nullptr;
}

int main() {
    tree_node<int> one(2);
    tree_node<int> two(3, &one);
    tree_node<int> three(1, nullptr, &two);
    tree_node<int> four(5);
    tree_node<int> root(4, &three, &four);

    BinaryTree<int> binarytree(&root);
    BinaryTree<int>::iterator iter = binarytree.begin();
    while (iter != binarytree.end()) {
        std::cout << *iter << std::endl;
        ++iter;
    }
    std::cout <<"count > 2" << std::endl;
    std::cout << std::count_if(binarytree.begin(), binarytree.end(), [](int a) { return a > 2; }) << std::endl;
    return 0;
}