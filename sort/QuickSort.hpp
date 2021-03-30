//
// Created by 孟祥鹏 on 2020/11/28.
//

#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP


#include "ISort.hpp"

template<typename T>
class QuickSort : public ISort<T> {

public:
    void sortIterative(T *arr, const int len) {
        quickSortIterative(arr, len);
    }

    void sortRecursive(T *arr, const int len) {
        quickSortRecursive(arr, len);
    }

    //三路归并排序
    void sortAdvanced(T *arr, const int len) {
        quickSort3Ways(arr, len);
    }

public:
    void quickSortIterative(T *arr, const int len) {
        quickSortIterative(arr, 0, len - 1);
        // assert(isSorted(arr, len));
    }

    void quickSortRecursive(T *arr, const int len) {
        quickSortRecursive(arr, 0, len - 1);
        // assert(isSorted(arr, len));
    }

    void quickSort3Ways(T *arr, const int len) {
        quickSort3Ways(arr, 0, len - 1);
        // assert(isSorted(arr, len));
    }


public:
    void quickSortIterative(T *arr, int lo, int hi) {
        //在堆上开内存
        int *stack = new int[hi - lo + 1];
        // 初始化栈顶变量
        int top = -1;

        // 将左右的边界值放在栈中
        stack[++top] = lo;
        stack[++top] = hi;

        // 当栈不空，就一直做下去
        while (top >= 0) {
            // 弹出刚刚记录的左右边界值
            hi = stack[top--];
            lo = stack[top--];

            int p = partitionAdvance(arr, lo, hi);

            // 如果左侧还有元素，则将这个左右边界压到栈中
            if (p - 1 > lo) {
                stack[++top] = lo;
                stack[++top] = p - 1;
            }

            // 如果右侧还有元素，则将这个左右边界压到栈中
            if (p + 1 < hi) {
                stack[++top] = p + 1;
                stack[++top] = hi;
            }
        }

    }


    //快速排序切换到插入排序阈值
    int QUICK2INSERT_THREAD = 24;

    void quickSortRecursive(T *arr, int lo, int hi) {
        //base case
        if (hi - lo <= QUICK2INSERT_THREAD) {
            if (QUICK2INSERT_THREAD > 0)
                insertSortAdvanced(arr, lo, hi);
            return;
        }
        //通过partition操作，将数组分成两个独立的部分
        //arr[lo...p-1] <= arr[p] <= arr[p+1...hi]
        int p = partitionAdvance(arr, lo, hi);
        quickSortRecursive(arr, lo, p - 1);    //将左半部分arr[lo...p-1]排序
        // assert(isSorted(arr, lo, p - 1));
        quickSortRecursive(arr, p + 1, hi);    //将右半部分arr[p+1...hi]排序
        // assert(isSorted(arr, p + 1, hi));
    }

    void quickSort3Ways(T *arr, int lo, int hi) {
        if (hi - lo <= 16) {
            insertSortAdvanced(arr, lo, hi);
            return;
        }
        /*
         arr[lo...lt-1] < V     arr[lt, i-1] = V    arr[gt+1...hi] > V
        |———————————————————|—————————|—|————————|————————————|
        |        <V         |    =V   |e|   ?    |     >V     |
        |———————————————————|—————————|—|————————|————————————|
                             lt        i      gt
         * */
        //随机选择基准
        int index = rand() % (hi - lo) + lo;
        swap(arr, index, lo);
        T pivot = arr[lo];

        int lt = lo, gt = hi, i = lo + 1;
        while (i <= gt) {
            if (arr[i] == pivot)
                i++;
            else if (arr[i] < pivot) {
                swap(arr, i, lt);
                i++;
                lt++;
            } else {
                swap(arr, i, gt);
                gt--;
            }
        }
        //现在有arr[lo...lt-1] < arr[lt...gt] < arr[gt+1...hi]
        quickSort3Ways(arr, lo, lt - 1);
        // assert(isSorted(arr, lo, lt - 1));
        quickSort3Ways(arr, gt + 1, hi);
        // assert(isSorted(arr, gt + 1, hi));
    }

    int partitionAdvance(T *arr, int lo, int hi) {
        int randIndex = rand() % (hi - lo + 1) + lo;
        swap(arr, lo, randIndex);
        T pivot = arr[lo];
        int i = lo + 1, j = hi;
        while (true) {
            while (i <= hi && arr[i] <= pivot) i++;
            while (j >= lo + 1 && arr[j] >= pivot) j--;
            if (i >= j)
                break;
            swap(arr, i, j);
        }
        /* 最后退出时arr[i] >= pivot，arr[j] =< pivot，
        我们应该将小于pivot的数交换到第一个元素的位置，也就是
        交换arr[j]和arr[lo]
        */
        swap(arr, lo, j);
        return j;
    }

    int partitionBase(T *arr, int lo, int hi) {
        //选择基准
        T pivot = arr[lo];
        int i = lo + 1, j = lo;
        while (true) {
            if (arr[i] < pivot) {
                swap(arr, j + 1, i);
                j++;
            }
            i++;
            if (i > hi)
                break;
        }
        swap(arr, lo, j);
        return j;
    }




    void insertSortAdvanced(T *arr, const int lo, const int hi) {
        for (int i = lo + 1; i <= hi; i++) {
            insertOperationAdvance(arr, i);
            // assert(isSorted(arr, lo, i));
        }
    }

    inline void insertOperationAdvance(T *arr, int i) {
        T last = arr[i];    //arr[i]这个值会被覆盖，所以临时保存一份
        int j = i - 1;
        while (j >= 0 && last < arr[j]) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = last;
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


#endif //QUICKSORT_HPP
