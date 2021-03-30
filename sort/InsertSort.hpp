#ifndef INSERTSORT_HPP
#define INSERTSORT_HPP

#include "ISort.hpp"

template<typename T>
class InsertSort : public ISort<T> {
public:
    void sortIterative(T *arr, const int len) {
        insertSortIterative(arr, len);
    }

    void sortRecursive(T *arr, const int len) {
        //insertionSortRecursive(arr, len);
        binaryInsertSort(arr, len);
    }

    void sortAdvanced(T *arr, const int len) {
        insertSortAdvanced(arr, len);
        binaryInsertSort(arr, len);
    }

public:
    void insertSortIterative(T *arr, const int len) {
        insertSortIterative(arr, 0, len - 1);
        // assert(isSorted(arr, len));
    }

    void insertionSortRecursive(T *arr, const int len) {
        insertionSortRecursive(arr, 0, len - 1);
        // assert(isSorted(arr, len));
    }

    void insertSortAdvanced(T *arr, const int len) {
        insertSortAdvanced(arr, 0, len - 1);
        // assert(isSorted(arr, len));
    }

private:

    void insertSortIterative(T *arr, const int lo, const int hi) {
        //arr[lo...i)有序，第一个元素arr[lo]自己不用排
        for (int i = lo + 1; i <= hi; i++) {
            insertOperation(arr, i);
            // assert(isSorted(arr, lo, i));
        }
    }

    void insertionSortRecursive(T *arr, const int lo, const int hi) {
        // Base case
        if (lo >= hi)
            return;
        // 对前len-1个数组进行排序
        insertionSortRecursive(arr, lo, hi - 1);
        // 此时arr[lo...hi-1]都是有序的
        // assert(isSorted(arr, lo, hi - 1));
        //只要将最后一个元素arr[hi]插入到前面有序数组中即可。
        insertOperationAdvance(arr, hi);
    }

    void insertSortAdvanced(T *arr, const int lo, const int hi) {
        for (int i = lo + 1; i <= hi; i++) {
            insertOperationAdvance(arr, i);
            // assert(isSorted(arr, lo, i));
        }
    }

    inline void insertOperation(T *arr, int i) {
        //将a[i]插入到a[i-1]、a[i-2]...之中，直到arr[j] >= arr[j-1]
        for (int j = i; j > 0 && arr[j] < arr[j - 1]; j--)
            swap(arr, j, j - 1);
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

    //nums[0...len)有序，在其中搜索插入位置，使用二分法
    int searchInsertPos(T *nums, const int len, T target) {
        int left = 0;
        // 因为有可能数组的最后一个元素的位置的下一个是我们要找的，故右边界是 len
        int right = len;
        while (left < right) {
            int mid = left + (right - left) / 2;
            // 小于 target 的元素一定不是解
            if (nums[mid] < target) {
                // 下一轮搜索的区间是 [mid + 1, right]
                left = mid + 1;
            } else {
                // 下一轮搜索的区间是 [left, mid]
                right = mid;
            }
        }
        return left;
    }

    void binaryInsertSort(T *arr, const int len) {
        for (int i = 1; i < len; i++) {
            T target = arr[i];
            int p = searchInsertPos(arr, i, target);
            //把[p...i-1]位置的都移动到[p+1...i]这个位置
            for (int j = i - 1; j >= p; j--)
                arr[j + 1] = arr[j];
            arr[p] = target;
        }
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

#endif /* INSERTSORT_HPP */