#include <iostream>
#include <vector>
#include <cassert>
#include <functional>

/*
 *                        A[0...7](0)
 *
 *        A[0...3](1)                    A[4...7](2)
 *
 *   A[0...1]     A[2...3]      A[4...5]         A[6...7]
 *
 * A[0]    A[1]  A[2]   A[3]   A[4]    A[5]    A[6]     A[7]
 *
 * data[i]
 *
 * */
template<typename T>
class SegmentTree {

public:
    SegmentTree(std::vector<T> arr, T (*merge)(T, T)) : data(arr), tree(std::vector<T>(4 * arr.size())), merge(merge) {
        //使用线段树，需要开4倍的空间
        buildSegmentTree(0, 0, data.size() - 1);
    }

    int size() const { return data.size(); }

    T get(int index) const {
        assert(index >= 0 && index < data.size());
        return data[index];
    }

    T query(int queryL, int queryR) const {
        assert(queryL >= 0 && queryL < data.size());
        assert(queryR >= 0 && queryR < data.size());
        assert(queryL <= queryR);
        return query(0, 0, data.size() - 1, queryL, queryR);
    }

    void set(int index, T e) {
        assert(index >= 0 && index < data.size());
        data[index] = e;
        set(0, 0, data.size() - 1, index, e);
    }

private:
    std::function<T(const T, const T)> merge;

    std::vector<T> data;
    std::vector<T> tree;

    int leftChild(int index) const { return index * 2 + 1; }

    int rightChild(int index) const { return index * 2 + 2; }

    void buildSegmentTree(int treeIndex, int dataLeft, int dataRight) {
        if (dataLeft == dataRight) {
            tree[treeIndex] = data[dataLeft];
            return;
        }
        int leftTreeIndex = leftChild(treeIndex);
        int rightTreeIndex = rightChild(treeIndex);

        int dataMid = ((dataRight - dataLeft) >> 1) + dataLeft;
        //[dataLeft, dataMid]
        //[dataMid+1, dataRight]
        buildSegmentTree(leftTreeIndex, dataLeft, dataMid);
        buildSegmentTree(rightTreeIndex, dataMid + 1, dataRight);

        tree[treeIndex] = merge(tree[leftTreeIndex], tree[rightTreeIndex]);
    }

    void set(int treeIndex, int begin, int end, int index, T e) {
        if (begin == end) {
            tree[treeIndex] = e;
            return;
        }
        int mid = ((end - begin) >> 1) + begin;
        int leftTreeIndex = leftChild(treeIndex);
        int rightTreeIndex = rightChild(treeIndex);

        if (index >= mid + 1)
            set(rightTreeIndex, mid + 1, end, index, e);
        else //if(index <= mid)
            set(leftTreeIndex, begin, mid, index, e);

        tree[treeIndex] = merge(tree[leftTreeIndex], tree[rightTreeIndex]);
    }

    //在index,[left, right]中查找  [queryL, queryR]
    T query(int treeIndex, int begin, int end, const int queryL, const int queryR) const {
        if (begin == queryL && end == queryR)
            return tree[treeIndex];
        int leftTreeIndex = leftChild(treeIndex);
        int rightTreeIndex = rightChild(treeIndex);

        int mid = ((end - begin) >> 1) + begin;

        if (queryL >= mid + 1)
            return query(rightTreeIndex, mid + 1, end, queryL, queryR);
        else if (queryR <= mid)
            return query(leftTreeIndex, begin, mid, queryL, queryR);
        else {
            T leftRes = query(leftTreeIndex, begin, mid, queryL, mid);
            T rightRes = query(rightTreeIndex, mid + 1, end, mid + 1, queryR);
            return merge(leftRes, rightRes);
        }
    }

    template<typename T1>
    friend std::ostream &operator<<(std::ostream &os, const SegmentTree<T1> &st);
};

template<typename T>
std::ostream &operator<<(std::ostream &os, const SegmentTree<T> &st) {
    os << "data :  [";
    for (int i = 0; i < st.data.size(); i++) {
        os << st.data[i];
        if (i != st.data.size() - 1)
            os << ",  ";
    }
    os << "]" << std::endl;

    os << "tree :  [";
    for (int i = 0; i < st.tree.size(); i++) {
        if (st.tree[i] == T())
            os << "NULL";
        else
            os << st.tree[i];
        if (i != st.tree.size() - 1)
            os << ",  ";
    }
    os << "]";
    return os;
}

int main() {
    std::vector<int> arr{0, 1, 2, 3, 4, 5, 6, 7};

    SegmentTree<int> st(arr, [](int a, int b) { return a * b; });

    std::cout << st << std::endl;

    std::cout << st.query(0, 5) << std::endl;


    st.set(3, 8);
    std::cout << st.query(2, 5) << std::endl;
    std::cout << st << std::endl;
    return 0;
}