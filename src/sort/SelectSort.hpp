#ifndef SELECTSORT_HPP
#define SELECTSORT_HPP


template<typename T>
class SelectSort {
public:
    void sortIterative(T *arr, const int len) {
        selectSortIterative(arr, len);
    }

    void sortRecursive(T *arr, const int len) {
        selectSortRecursive(arr, len);
    }

    void sortAdvanced(T *arr, const int len) {
        selectSortAdvanced(arr, len);
    }

public:
    void selectSortIterative(T *arr, const int len) {
        selectSortIterative(arr, 0, len - 1);
    }

    void selectSortRecursive(T *arr, const int len) {
        selectSortRecursive(arr, 0, len - 1);
    }

    void selectSortAdvanced(T *arr, const int len) {
        selectSortAdvanced(arr, 0, len - 1);
    }


private:
    void selectSortIterative(T *arr, const int lo, const int hi) {
        for (int i = lo; i <= hi; i++) {
            int min = selectMinIndex(arr, i, hi);
            swap(arr, min, i);
            // assert(isSorted(arr, lo, i));
        }
    }

    void selectSortRecursive(T *arr, const int lo, const int hi) {
        if (lo == hi)
            return;
        //完成arr[index]元素的排序，在这之前的arr[0...index-1]都是有序的
        int k = selectMinIndexRecursive(arr, lo, hi);
        swap(arr, k, lo);
        // assert(isSorted(arr, 0, lo));
        //使用递归完成arr[index+1, len)的排序
        selectSortRecursive(arr, lo + 1, hi);
    }

    void selectSortAdvanced(T *arr, const int lo, const int hi) {
        for (int i = lo; i <= hi; i++) {
            int min = selectMinIndex(arr, i, hi);
            if (min != i)
                swap(arr, min, i);
            // assert(isSorted(arr, lo, i));
        }
    }

    inline int selectMinIndexRecursive(T *arr, const int lo, const int hi) {
        if (lo == hi)
            return lo;
        int tmp = selectMinIndexRecursive(arr, lo + 1, hi);
        return (arr[tmp] < arr[lo]) ? tmp : lo;
    }

    //在arr[lo...hi]选择最小值，并返回最小值所在的下标
    inline int selectMinIndex(T *arr, const int lo, const int hi) {
        int min = lo;
        for (int i = lo + 1; i <= hi; i++)
            min = (arr[i] < arr[min]) ? i : min;
        return min;
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

#endif //SELECTSORT_HPP