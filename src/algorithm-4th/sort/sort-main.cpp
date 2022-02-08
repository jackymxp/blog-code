#include <iostream>
#include <sys/time.h>
#include <cmath>
#include <algorithm>
#include <thread>
#include <functional>
#include<future>      //std::future std::promise
#include<utility>     //std::ref
#include<chrono>      //std::chrono::seconds

#include "QuickSort.hpp"
#include "HeapSort.hpp"
#include "MergeSort.hpp"
#include "QuickSortMuiltThread.hpp"
#include "InsertSort.hpp"

using namespace std;


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
double test_sort_function(const string& sortName, T* arr, const int len, const function<void(decltype(arr), decltype(len))>&sort_function) {
    struct timeval start, end;
    double duration = 0;
    gettimeofday(&start, NULL);

    sort_function(arr, len);

    gettimeofday(&end, NULL);

    duration = ((end.tv_sec - start.tv_sec) * pow(10, 6) + (end.tv_usec - start.tv_usec)) / pow(10, 6);

    bool res = isSorted(arr, len);
    cout << sortName << (duration < 1 ? duration * 1000 : duration) << ((duration < 1) ? "ms" : " s")
         << ", result = " << (res == true ? ("true") : ("false")) << endl;
    return duration;
}

template<typename sort_alg, typename T>
void test_sort(const string& sortName, sort_alg&& sort, T* arr, const int len) {

    auto sort_functions = {
        bind(&sort_alg::sortIterative, sort, std::placeholders::_1, std::placeholders::_2),
        bind(&sort_alg::sortRecursive, sort, std::placeholders::_1, std::placeholders::_2),
        bind(&sort_alg::sortAdvanced, sort, std::placeholders::_1, std::placeholders::_2),
    };

    for(const auto& fun : sort_functions) {
        T * backArr = cloneArray(arr, len);
        test_sort_function(sortName + " 迭代: ", backArr, len, fun);
        delete[] backArr;
    }
}



namespace test_thread_quick_sort {

    template<typename T>
    int partitionAdvance(T *arr, int lo, int hi) {
        int randIndex = rand() % (hi - lo + 1) + lo;
        swap(arr[lo], arr[randIndex]);

        T pivot = arr[lo];
        int i = lo + 1, j = hi;
        while (true) {
            while (i <= hi && arr[i] <= pivot) i++;
            while (j >= lo + 1 && arr[j] >= pivot) j--;
            if (i >= j)
                break;
            //swap(arr, i, j);
            swap(arr[i], arr[j]);
        }
        /* 最后退出时arr[i] >= pivot，arr[j] =< pivot，
        我们应该将小于pivot的数交换到第一个元素的位置，也就是
        交换arr[j]和arr[lo]
        */
        //swap(arr, lo, j);
        swap(arr[lo], arr[j]);
        return j;
    }

    template<typename T>
    void sortSingeleThread(T* arr, const int lo, const int hi) {
        cout << "thread ID " << std::this_thread::get_id() << endl;
        std::sort(arr+lo, arr+hi+1);
    }

    template<typename T>
    void twoThreadQuickSort(T* arr, const int lo, const int hi){
        int p = partitionAdvance(arr, lo, hi);
        //cout << (hi+lo) << " <<>> " << p << endl;
        thread t1 = thread(sortSingeleThread<T>, arr, lo, p-1);
        thread t2 = thread(sortSingeleThread<T>, arr, p+1, hi);
        cout << "1111111111" << endl;
        t1.join();
        cout << "2222222222" << endl;
        t2.join();
        cout << "3333333333" << endl;
    }

    template<typename T>
    void twoThreadQuickSort(T* arr, const int len){
        twoThreadQuickSort(arr, 0, len - 1);
    }

    template<typename T>
    void stdSortVSMuiltThreadSort(T* arr, const int len) {
      
        T* copy;

        copy = cloneArray(arr, len);
        
        struct timeval start, end;
        double duration = 0;
        bool res;
        gettimeofday(&start, NULL);
        test_thread_quick_sort::twoThreadQuickSort(copy, len);
        gettimeofday(&end, NULL);

        duration = ((end.tv_sec - start.tv_sec) * pow(10, 6) + (end.tv_usec - start.tv_usec)) / pow(10, 6);
        res = isSorted(copy, len);
        cout << "多线程快速排序" << (duration < 1 ? duration * 1000 : duration) << ((duration < 1) ? "ms" : " s")
        << ", result = " << (res == true ? ("true") : ("false")) << endl;
        delete[] copy;


        copy = cloneArray(arr, len);
        gettimeofday(&start, NULL);
        sort(arr, arr+len/2);
        gettimeofday(&end, NULL);

        duration = ((end.tv_sec - start.tv_sec) * pow(10, 6) + (end.tv_usec - start.tv_usec)) / pow(10, 6);
        res = isSorted(arr, len);
        cout << "std::sort" << (duration < 1 ? duration * 1000 : duration) << ((duration < 1) ? "ms" : " s")
        << ", result = " << (res == true ? ("true") : ("false")) << endl;

        delete[] copy;
    }



} // test_thread_quick_sort

int main(void) {
    const int num = 1000;
    const int len = 10000 * num;
    int *arr;
    //arr = generateRandomArray<int>(len, 0, len);
    int* copy;
    while(1){
        // arr = generateSortedArray<int>(len, false);
        arr = generateRandomArray<int>(len, 0, len);
        //test_sort("快速排序", QuickSort<int>(), arr, len);
        //test_sort("堆排序", HeapSort<int>(), arr, len);
        //test_sort("归并排序", MergeSort<int>(), arr, len);
        //test_sort("多线程快速排序", QuickSortMuiltThread<int>(), arr, len);
        
        // auto t = test_sort_function("插入排序 改进: ", arr, len, bind(&InsertSort<int>::sortAdvanced, InsertSort<int>(), std::placeholders::_1, std::placeholders::_2));

        std::thread qt([=](){test_sort("快速排序", QuickSort<int>(), arr, len);});
        std::thread ht([=](){test_sort("堆排序", HeapSort<int>(), arr, len);});
        std::thread mt([=](){test_sort("归并排序", MergeSort<int>(), arr, len);});
        std::thread mqt([=](){test_sort("多线程快速排序", QuickSortMuiltThread<int>(), arr, len);});

        qt.join();
        ht.join();
        mt.join();
        mqt.join();
        delete[] arr;
    }
    return 0;
}

