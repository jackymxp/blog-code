#ifndef BUBBLESORT_HPP
#define BUBBLESORT_HPP

#include "ISort.hpp"

template<typename T>
class BubbleSort : public ISort<T> {
public:
    void sortIterative(T *arr, const int len) {
        bubbleSortIterative(arr, len);
    }


    void sortRecursive(T *arr, const int len) {
        bubbleSortRecursive(arr, len);
    }

    void sortAdvanced(T *arr, const int len) {
        bubbleSortAdvanced(arr, len);
    }

public:
    void bubbleSortIterative(T *arr, const int len) {
        bubbleSortIterative(arr, 0, len - 1);
        // assert(isSorted(arr, len) == true);
    }

    void bubbleSortRecursive(T *arr, const int len) {
        bubbleSortRecursive(arr, 0, len - 1);
        // assert(isSorted(arr, len) == true);
    }

    void bubbleSortAdvanced(T *arr, const int len) {
        bubbleSortAdvanced(arr, 0, len - 1);
        // assert(isSorted(arr, len) == true);
    }

private:
    void bubbleSortIterative(T *arr, const int lo, const int hi) {
        for (int i = hi; i >= lo+1; i--) {
            bubbleOperator(arr, i);
            // assert(isSorted(arr, i, hi));
        }
    }

    void bubbleSortRecursive(T *arr, const int lo, const int hi) {
        if (lo >= hi)
            return;
        bubbleOperator(arr, hi);
        bubbleSortRecursive(arr, lo, hi - 1);
    }

    void bubbleSortAdvanced(T *arr, const int lo, const int hi) {
        for (int i = hi; i >= lo+1; i--) {
            if(bubbleOperatorAdvanced(arr, i) == true)
                return ;
            // assert(isSorted(arr, i, hi));
        }
    }

    //在arr[0...hi]做冒泡过程，将最大值放在arr[hi]位置
    inline void bubbleOperator(T *arr, const int hi) {
        for (int i = 0; i < hi; i++)
            if (arr[i] > arr[i + 1])
                swap(arr, i, i + 1);
    }

    //没有发生交换返回true
    inline bool bubbleOperatorAdvanced(T *arr, const int hi) {
        bool isSort = true;
        for (int i = 0; i < hi; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr, i, i + 1);
                isSort = false;
            }
        }
        return isSort;
    }

    void swap(T *arr, const int i, const int j) {
        T k = arr[i];
        arr[i] = arr[j];
        arr[j] = k;
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


#endif //BUBBLESORT_HPP
