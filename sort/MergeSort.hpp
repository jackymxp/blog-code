#ifndef MERGESORT_HPP
#define MERGESORT_HPP


#include "ISort.hpp"

using namespace std;

template<typename T>
class MergeSort : public ISort<T> {
public:
    void sortIterative(T *arr, const int len) {
        mergeSortIterative(arr, len);
    }

    void sortRecursive(T *arr, const int len) {
        mergeSortRecursive(arr, len);
    }

    void sortAdvanced(T *arr, const int len) {
        mergeSort3Way(arr, len);
    }

    void mergeSortIterative(T *arr, const int len) {
        T *aux = new T[len];
        //首先归并长度为1的，将其变成2；然后我们应该归并长度为2的，将其变成4。所以有sz+sz
        for (int sz = 1; sz < len; sz = sz + sz) {//执行归并过程merge[lo, lo+sz, lo+2sz]
            for (int lo = 0; lo < len - sz; lo += sz + sz) {
                //这里因为lo+sz+sz可能超出了数组范围，所以进行一下比较。
                merge2Ways(arr, lo, lo + sz - 1, min(lo + sz + sz - 1, len - 1), aux);
                // assert(isSorted(arr, lo, lo+sz-1));
                // assert(isSorted(arr, lo+sz, min(lo+sz+sz-1, len - 1)));
            }
        }
        delete[] aux;
        // assert(isSorted(arr, len));
    }

    void mergeSortRecursive(T *arr, const int len) {
        mergeSortRecursive(arr, 0, len - 1);
        // assert(isSorted(arr, len));
    }

    void mergeSort3Way(T *arr, const int len) {
        mergeSort3Way(arr, 0, len - 1);
        // assert(isSorted(arr, len));
    }

private:
    void mergeSortRecursive(T *arr, const int lo, const int hi) {
        T *aux = new T[hi - lo + 1];
        mergeSortRecursive(arr, lo, hi, aux);
        delete[] aux;
    }

    void mergeSortRecursive(T *arr, const int lo, const int hi, T *aux) {
        if (lo >= hi)
            return;

        int mid = ((hi - lo) >> 1) + lo;
        mergeSortRecursive(arr, lo, mid, aux);    //排序前半段
        // assert(isSorted(arr, lo, mid));
        mergeSortRecursive(arr, mid + 1, hi, aux);    //排序后半段
        // assert(isSorted(arr, mid+1, hi));
        merge2Ways(arr, lo, mid, hi, aux); //将有序的前半段和有序的后半段合并起来
    }

    void merge2Ways(T *arr, const int lo, const int mid, const int hi, T *aux) {
        //对arr[lo...mid]和arr[mid+1...hi] 归并
        for (int k = lo; k <= hi; k++)
            aux[k] = arr[k];
        int i = lo, j = mid + 1, k = lo;
        while (i <= mid && j <= hi) {
            //arr[k++] = aux[i] < aux[j] ? aux[i++] : aux[j++];
            if (aux[i] < aux[j])
                arr[k++] = aux[i++];
            else
                arr[k++] = aux[j++];
        }
        while (i <= mid)
            arr[k++] = aux[i++];
        while (j <= hi)
            arr[k++] = aux[j++];
    }

    void mergeSort3Way(T *arr, int lo, int hi) {
        if (hi - lo <= 16) {
            insertSortAdvanced(arr, lo, hi);
            return;
        }
        T *aux = new T[hi - lo + 1];
        mergeSort3Way(arr, lo, hi, aux);
        delete[] aux;
    }

    void mergeSort3Way(T *arr, int lo, int hi, T *aux) {
        if (lo >= hi)
            return;
        int mid1 = lo + ((hi - lo) / 3);
        int mid2 = lo + 2 * ((hi - lo) / 3);
        //分成三段
        mergeSort3Way(arr, lo, mid1, aux);
        // assert(isSorted(arr, lo, mid1));
        mergeSort3Way(arr, mid1 + 1, mid2, aux);
        // assert(isSorted(arr, mid1+1, mid2));
        mergeSort3Way(arr, mid2 + 1, hi, aux);
        // assert(isSorted(arr, mid2+1, hi));
        //对三段数组进行合并
        merge3Ways(arr, lo, mid1, mid2, hi, aux);
    }

    void merge3Ways(T *arr, int lo, int mid1, int mid2, int hi, T *aux) {
        for (int i = lo; i <= hi; i++)
            aux[i] = arr[i];
        int i = lo, j = mid1 + 1, k = mid2 + 1, l = lo;
        //三个值相互比较
        while ((i <= mid1) && (j <= mid2) && (k <= hi)) {
            if (aux[i] < aux[j])
                arr[l++] = (aux[i] < aux[k]) ? aux[i++] : aux[k++];
            else
                arr[l++] = (aux[j] < aux[k]) ? aux[j++] : aux[k++];
        }
        //二个值相互比较
        while ((i <= mid1) && (j <= mid2))
            arr[l++] = (aux[i] < aux[j]) ? aux[i++] : aux[j++];
        while ((j <= mid2) && (k <= hi))
            arr[l++] = (aux[j] < aux[k]) ? aux[j++] : aux[k++];
        while ((i <= mid1) && (k <= hi))
            arr[l++] = (aux[i] < aux[k]) ? aux[i++] : aux[k++];
        //剩余一个直接填充
        while (i <= mid1)
            arr[l++] = aux[i++];
        while (j <= mid2)
            arr[l++] = aux[j++];
        while (k <= hi)
            arr[l++] = aux[k++];
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


#endif  //MERGESORT_HPP