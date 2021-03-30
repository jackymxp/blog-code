#ifndef MERGESORTMUILTSORT_HPP
#define MERGESORTMUILTSORT_HPP

#include "ISort.hpp"
#include <vector>
#include <thread>
#include <queue>
#include <ctime>
#include <functional>
using namespace std;

template<typename T>
class MergeSortMuiltThread : public ISort<T> {

public:

    void sortIterative(T *arr, const int len) {
        muiltThreadMergeSortSingleThreadMerge(arr, len);
    };

    void sortRecursive(T *arr, const int len) {
        muiltThreadMergeSortSingleThreadMerge(arr, len);
    };

    void sortAdvanced(T *arr, const int len) {
        muiltThreadMergeSortTwoThreadMerge(arr, len);
    }

public:
    int threadNum = 8;


    void muiltThreadMergeSortSingleThreadMerge(T *arr, const int len) {
        auto table = initTable(arr, len);
        muiltThreadMergeSortCore(table);
        mergeKSortArraySingleThread(table, arr, len);
    }

    void muiltThreadMergeSortTwoThreadMerge(T *arr, const int len) {
        auto table = initTable(arr, len);
        muiltThreadMergeSortCore(table);
        mergeKSortArrayTwoThread(table, arr, len);
    }

    //存放拆分的数组和该数组的大小
    vector<pair<T *, int>> initTable(T *arr, const int len) {
        vector<pair<T *, int>> table;
        int mod = len % threadNum;
        int step = len / threadNum;
        int k = 0;
        for (int i = 0; i < threadNum; i++) {
            int t = (i < mod ? 1 : 0);
            T *tmp = new T[step + t];
            for (int j = 0; j < (step + t); j++)
                tmp[j] = arr[k++];
            table.push_back(make_pair(tmp, step + t));
        }
        return table;
    }

    //合并二维数组中的一维数组，使得每一个数组都是有序的，然后就是合并K个有序数组的过程
    //Leetcode 合并K个有序链表  https://leetcode-cn.com/problems/merge-k-sorted-lists/
    void muiltThreadMergeSortCore(const vector<pair<T *, int>> &table) {
        thread t[threadNum];
        for (int i = 0; i < threadNum; i++)
            t[i] = thread(
                    std::bind(&MergeSortMuiltThread::mergeSortSingleThread, this, table[i].first, table[i].second));

        for (int i = 0; i < threadNum; i++)
            t[i].join();
    }

    struct Node {
        T *arr;
        int index;
        int size;

        Node(T *arr, int index, int size) : arr(arr), index(index), size(size) {}

        bool operator>(const Node &o) const {
            return arr[index] < o.arr[o.index];
        }

        bool operator<(const Node &o) const {
            return arr[index] > o.arr[o.index];
        }
    };


    void mergeKSortArraySingleThread(const vector<pair<T *, int>> &table, T *arr, int len) {
        priority_queue<Node, vector<Node>, less<Node> > minHeap;

        for (int i = 0; i < threadNum; i++)
            minHeap.push(Node(table[i].first, 0, table[i].second));

        int k = 0;
        int threadHold = len;
        while (k < threadHold) {
            Node node = minHeap.top();
            minHeap.pop();
            arr[k++] = node.arr[node.index];
            if (node.index + 1 < node.size) {
                node.index++;
                minHeap.push(node);
            }
        }
    }


    void mergeKSortArrayTwoThread(const vector<pair<T *, int>> &table, T *arr, int len) {
        thread t1 = thread(&MergeSortMuiltThread::mergeKSortArrayLeftHalf, this, table, arr, len);
        thread t2 = thread(&MergeSortMuiltThread::mergeKSortArrayRightHalf, this, table, arr, len);
        t1.join();
        t2.join();
    }

    void mergeKSortArrayLeftHalf(const vector<pair<T *, int>> &table, T *arr, int len) {
        priority_queue<Node, vector<Node>, less<Node> > minHeap;
        for (int i = 0; i < threadNum; i++) {
            minHeap.push(Node(table[i].first, 0, table[i].second));
        }

        int k = 0;
        int threadHold = len / 2;
        while (k < threadHold) {
            Node node = minHeap.top();
            minHeap.pop();
            arr[k++] = node.arr[node.index];
            if (node.index + 1 < node.size) {
                node.index++;
                minHeap.push(node);
            }
        }
    }

    void mergeKSortArrayRightHalf(const vector<pair<T *, int>> &table, T *arr, int len) {
        priority_queue<Node, vector<Node>, greater<Node> > maxHeap;

        for (int i = 0; i < threadNum; i++) {
            T *arr = table[i].first;
            int size = table[i].second;
            maxHeap.push(Node(arr, size - 1, size));
        }
        int k = len - 1;
        int threadHold = len / 2;
        while (k >= threadHold) {
            Node node = maxHeap.top();
            maxHeap.pop();
            arr[k--] = node.arr[node.index];
            if (node.index - 1 >= 0) {
                node.index--;
                maxHeap.push(node);
            }
        }
    }

private:
    MergeSort<T> *mergeSort = new MergeSort<T>();
    void mergeSortSingleThread(T *arr, const int len) {
        //mergeSort->mergeSort3Way(arr, len);
        std::sort(arr, arr+len);
    }

    bool isSorted(T *arr, const int lo, const int hi) {
        for (int i = lo; i < hi; i++)
            if (arr[i] > arr[i + 1])
                return false;
        return true;
    }

    bool isSorted(T *arr, const int len) {
        return isSorted(arr, 0, len - 1);
    }
};

#endif //MERGESORTMUILTSORT_HPP
