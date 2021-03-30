#include <iostream>
#include <algorithm>

#include "ISort.hpp"
#include "BubbleSort.hpp"
#include "SelectSort.hpp"
#include "InsertSort.hpp"
#include "ShellSort.hpp"
#include "HeapSort.hpp"
#include "QuickSort.hpp"
#include "MergeSort.hpp"
#include "MergeSortMuiltThread.hpp"
#include "QuickSortMuiltThread.hpp"
#include <vector>
#include <sys/time.h>
#include <algorithm>

template<typename T>
bool isSorted(T *arr, const int lo, const int hi) {
    for (int i = lo; i < hi; i++)
        if (arr[i] > arr[i + 1])
            return false;
    return true;
}

template<typename T>
bool isSorted(T *arr, const int len) {
    return isSorted(arr, 0, len - 1);
}


template<typename T>
T *cloneArray(T *arr, const int len) {
    T *dst = new T[len];
    for (int i = 0; i < len; i++)
        dst[i] = arr[i];
    return dst;
}

template<typename T>
T *generateRandomArray(int num, int lo, int hi) {
    T *arr = new T[num];
    for (int i = 0; i < num; i++)
        arr[i] = rand() % (hi - lo + 1) + lo;
    return arr;
}

template<typename T>
T *generateSortedArray(int num, bool reverse) {
    T *arr = new T[num];
    if (reverse) {
        for (int i = 0; i < num; i++)
            arr[i] = i;
    } else {
        for (int i = 0; i < num; i++)
            arr[i] = num - i - 1;
    }

    return arr;
}

template<typename T>
T *generateNearlyRandomArray(int num, int swapNum) {
    T *arr = new T[num];
    for (int i = 0; i < num; i++)
        arr[i] = i;
    for (int i = 0; i < swapNum; i++) {
        int a = rand() % num;
        int b = rand() % num;
        swap(arr[a], arr[b]);
    }
    return arr;
}


template<typename T>
bool testSortAlgorithm(string sortName, ISort<T> *sort, T *arr, const int len) {
    T *arr1 = cloneArray(arr, len);
    T *arr2 = cloneArray(arr, len);
    T *arr3 = cloneArray(arr, len);
    T *arr4 = cloneArray(arr, len);

    struct timeval start, end;
    double duration = 0;

    gettimeofday(&start, NULL);
    sort->sortIterative(arr1, len);
    gettimeofday(&end, NULL);
    bool res1 = isSorted(arr1, len);
    duration = ((end.tv_sec - start.tv_sec) * pow(10, 6) + (end.tv_usec - start.tv_usec)) / pow(10, 6);
    cout << sortName << " 迭代 " << (duration < 1 ? duration * 1000 : duration) << ((duration < 1) ? "ms" : " s")
         << ", result = "\
 << (res1 == true ? ("true") : ("false")) << endl;

    gettimeofday(&start, NULL);
    sort->sortRecursive(arr2, len);
    gettimeofday(&end, NULL);
    bool res2 = isSorted(arr2, len);
    duration = ((end.tv_sec - start.tv_sec) * pow(10, 6) + (end.tv_usec - start.tv_usec)) / pow(10, 6);
    cout << sortName << " 递归 " << (duration < 1 ? duration * 1000 : duration) << ((duration < 1) ? "ms" : " s")
         << ", result = "\
 << (res2 == true ? ("true") : ("false")) << endl;

    gettimeofday(&start, NULL);
    sort->sortAdvanced(arr3, len);
    gettimeofday(&end, NULL);
    bool res3 = isSorted(arr3, len);
    duration = ((end.tv_sec - start.tv_sec) * pow(10, 6) + (end.tv_usec - start.tv_usec)) / pow(10, 6);
    cout << sortName << " 改进 " << (duration < 1 ? duration * 1000 : duration) << ((duration < 1) ? "ms" : " s")
         << ", result = "\
 << (res3 == true ? ("true") : ("false")) << endl;


    gettimeofday(&start, NULL);
    std::sort(arr4,arr4+len);
    gettimeofday(&end, NULL);
    bool res4 = isSorted(arr3, len);
    duration = ((end.tv_sec - start.tv_sec) * pow(10, 6) + (end.tv_usec - start.tv_usec)) / pow(10, 6);
    cout << "STL:sort()排序 " << (duration < 1 ? duration * 1000 : duration) << ((duration < 1) ? "ms" : " s")
         << ", result = "\
 << (res4 == true ? ("true") : ("false")) << endl;
    cout << endl;

    return res1 && res2 && res3;
}

enum SORT_ALG {
    BUBBLE_SORT,
    SELECT_SORT,
    INSERT_SORT,
    SHELL_SORT,

    HEAP_SORT,
    MERGE_SORT,
    QUICK_SORT,

    MUILTTHREAD_MERGE_SORT,
    MUILTTHREAD_QUICK_SORT,
};

template<typename T>
pair<ISort<T> *, string> sortType2SortAlgAndSortName(SORT_ALG type) {
    ISort<T> *sortAlg;
    string sortName;
    switch (type) {
        case BUBBLE_SORT:
            sortAlg = new BubbleSort<T>();
            sortName = "BubbleSort";
            break;
        case SELECT_SORT:
            sortAlg = new SelectSort<T>();
            sortName = "SelectSort";
            break;
        case INSERT_SORT:
            sortAlg = new InsertSort<T>();
            sortName = "InsertSort";
            break;
        case SHELL_SORT:
            sortAlg = new ShellSort<T>();
            sortName = "ShellSort";
            break;

        case HEAP_SORT:
            sortAlg = new HeapSort<T>();
            sortName = "HeapSort";
            break;
        case MERGE_SORT:
            sortAlg = new MergeSort<T>();
            sortName = "MergeSort";
            break;
        case QUICK_SORT:
            sortAlg = new QuickSort<T>();
            sortName = "QuickSort";
            break;

        case MUILTTHREAD_MERGE_SORT:
            sortAlg = new MergeSortMuiltThread<T>();
            sortName = "MergeSortMuiltThread";
            break;
        case MUILTTHREAD_QUICK_SORT:
            sortAlg = new QuickSortMuiltThread<T>();
            sortName = "QuickSortMuiltThread";
            break;
        default:
            sortAlg = nullptr;
            sortName = "error sort name";
    }
    auto res = make_pair(sortAlg, sortName);
    return res;
}

template<typename T>
bool testSortAlgorithm(T *arr, const int len, const vector<SORT_ALG> &types) {
    bool res = true;
    for (auto type : types) {
        auto p = sortType2SortAlgAndSortName<T>(type);
        ISort<T> *sortAlg = p.first;
        string sortName = p.second;
        res &= testSortAlgorithm(sortName, sortAlg, arr, len);
    }
    return res;
}


bool testSortAlgorithm() {
    const int num = 1000;
    const int len = 10000 * num;
    int *arr;
    //arr = generateSortedArray<int>(len, false);
    arr = generateRandomArray<int>(len, 0, len);
    //arr = generateNearlyRandomArray<int>(len, len / (len /100));

    vector<SORT_ALG> baseSortAlg = {BUBBLE_SORT, SELECT_SORT, INSERT_SORT, SHELL_SORT,};
    vector<SORT_ALG> allSortAlg = {BUBBLE_SORT, SELECT_SORT, INSERT_SORT, SHELL_SORT, HEAP_SORT, MERGE_SORT, QUICK_SORT,
                                   MUILTTHREAD_MERGE_SORT, MUILTTHREAD_QUICK_SORT,};
    vector<SORT_ALG> goodSortAlg = {SHELL_SORT, HEAP_SORT, MERGE_SORT, QUICK_SORT,};
    vector<SORT_ALG> betterSortAlg = {HEAP_SORT, MERGE_SORT, QUICK_SORT,};
    vector<SORT_ALG> muiltSortAlg = {MUILTTHREAD_MERGE_SORT, MUILTTHREAD_QUICK_SORT,};
    vector<SORT_ALG> singVsMuiltThreadSortAlg = {MERGE_SORT, QUICK_SORT, MUILTTHREAD_MERGE_SORT,
                                                 MUILTTHREAD_QUICK_SORT,};

    bool res = testSortAlgorithm(arr, len, singVsMuiltThreadSortAlg);
    return res;
}