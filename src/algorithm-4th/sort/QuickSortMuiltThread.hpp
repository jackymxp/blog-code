//
// Created by 孟祥鹏 on 2021/2/23.
//

#ifndef QUICKSORTMUILTTHREAD_HPP
#define QUICKSORTMUILTTHREAD_HPP

#include <algorithm>
#include <functional> 
#include <thread>

template<typename T>
class QuickSortMuiltThread {
public:

    void sortIterative(T *arr, const int len) {
        twoThreadQuickSort(arr, len);
    };

    void sortRecursive(T *arr, const int len) {
        twoThreadQuickSort(arr, len);
    };

    void sortAdvanced(T *arr, const int len) {
        fourThreadQuickSort(arr, len);
    }

private:
    void twoThreadQuickSort(T *arr, const int len) {
        twoThreadQuickSort(arr, 0, len - 1);
    }

    void twoThreadQuickSort(T *arr, const int lo, const int hi) {
        if(lo >= hi)
            return;
        
        int tl = (hi-lo) / 3 + lo;
        int tr = (hi-lo) / 3 * 2 + lo;
        int p = 0;
        while (p < tl || p > tr) {
            p = partitionAdvance(arr, lo, hi);
        }

        //开一个线程排序[0...p-1]
        thread t1 = thread(std::bind(&QuickSortMuiltThread::quickSortForSingleThread, this, arr, lo, p - 1));
        //开一个线程排序[p+1...len-1]
        thread t2 = thread(std::bind(&QuickSortMuiltThread::quickSortForSingleThread, this, arr, p + 1, hi));
        t1.join();
        t2.join();
    }

    void fourThreadQuickSort(T *arr, const int len) {
        fourThreadQuickSort(arr, 0, len - 1);
    }

    void fourThreadQuickSort(T *arr, const int lo, const int hi) {
        int tl = (hi-lo) / 3 + lo;
        int tr = (hi-lo) / 3 * 2 + lo;
        int p = 0;
        while (p < tl || p > tr) {
            p = partitionAdvance(arr, lo, hi);
        }
        twoThreadQuickSort(arr, lo, p - 1);
        twoThreadQuickSort(arr, p + 1, hi);
    }

    void quickSortForSingleThread(T *arr, int lo, int hi) {
        // quickSort3Ways(arr, lo, hi);
        std::sort(arr+lo, arr+hi+1);
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


#endif //QUICKSORTMUILTTHREAD_HPP
