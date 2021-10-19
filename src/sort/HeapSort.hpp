#ifndef HEAPSORT_HPP
#define HEAPSORT_HPP

template<typename T>
class HeapSort {
public:
    void sortIterative(T *arr, const int len) {
        heapSortIterative(arr, len);
        // assert(isSorted(arr, len));
    }

    void sortRecursive(T *arr, const int len) {
        heapSortRecursive(arr, len);
        // assert(isSorted(arr, len));
    }

    void sortAdvanced(T *arr, const int len) {
        DHeapSort(arr, len);
        // assert(isSorted(arr, len));
    }

private:
    void heapSortIterative(T *arr, const int len) {
        //buildHeapByShiftDownIterative(arr, len);
        buildHeapByShiftUpIterative(arr, len);
        for (int i = len - 1; i >= 0; i--) {
            swap(arr, i, 0);
            // assert(isSorted(arr, i, len - 1));
            //针对根节点在长度为i的树中做下沉操作
            shiftDownIterative(arr, 0, i);
        }
    }

    void heapSortRecursive(T *arr, const int len) {
        //这里首先要将一个无序数组组织成堆
        //buildHeapByShiftUpRecursive(arr, len);
        buildHeapByShiftDownRecursive(arr, len);
        for (int i = len - 1; i >= 0; i--) {
            swap(arr, i, 0);
            // assert(isSorted(arr, i, len - 1));
            //针对根节点在长度为i的树中做下沉操作
            shiftDownRecursive(arr, 0, i);
        }
    }

    void DHeapSort(T *arr, const int len) {
        buildDHeap(arr, len);
        for (int i = len - 1; i >= 0; i--) {
            swap(arr, 0, i);
            // assert(isSorted(arr, i, len - 1));
            ShiftDownInDHeap(arr, 0, i);
        }
    }


    void buildHeapByShiftUpIterative(T *arr, int len) {
        for (int i = 1; i < len; i++)
            shiftUpIterative(arr, i, len);
    }

    void buildHeapByShiftDownIterative(T *arr, int len) {
        for (int i = (len - 1) / 2; i >= 0; i--)
            shiftDownIterative(arr, i, len);
    }

    void buildHeapByShiftDownRecursive(T *arr, int len) {
        for (int i = (len - 1) / 2; i >= 0; i--)
            shiftDownRecursive(arr, i, len);
    }

    void buildHeapByShiftUpRecursive(T *arr, int len) {
        for (int i = 1; i < len; i++)
            shiftUpRecursive(arr, i, len);
    }

    void shiftUpRecursive(T *arr, int child, int len) {
        if (child <= 0)
            return;
        int parent = (child - 1) / 2;
        if (arr[parent] < arr[child]) {
            swap(arr, child, parent);
            shiftUpRecursive(arr, parent, len);
        }
    }

    void shiftUpIterative(T *arr, int child, int len) {
        while (child >= 0) {
            int parent = ((child - 1) >> 1);
            if (parent >= 0 && arr[parent] < arr[child])
                swap(arr, child, parent);
            child = parent;
        }
    }

    void shiftDownRecursive(T *arr, int parent, int len) {
        //base case 没有孩子节点
        int lo = (parent << 1) + 1;
        if (lo >= len)
            return;
        int hi = lo + 1;
        //选择父节点、左右孩子中最大的那个元素
        int max = parent;
        if (lo < len && arr[max] < arr[lo])
            max = lo;
        if (hi < len && arr[max] < arr[hi])
            max = hi;
        //如果父节点不是最大值，则交换，并对孩子节点接续做heapify
        if (parent != max) {
            swap(arr, parent, max);
            shiftDownRecursive(arr, max, len);
        }
    }

    void shiftDownIterative(T *arr, int parent, int len) {
        while (((parent << 1) + 1) < len) {
            int lo = (parent << 1) + 1;
            int hi = lo + 1;
            int child = lo;
            if (hi < len && arr[child] < arr[hi])
                child = hi;
            if (arr[parent] < arr[child])
                swap(arr, parent, child);
            parent = child;
        }
    }


    int Dchild = 5; //假设是5叉堆

    //在长度为len的数组中，调整parent，使其恢复堆的性质
    void ShiftDownInDHeap(T *arr, int parent, int len) {
        int lo = Dchild * parent + 1;
        //base case 没有孩子节点
        if (lo >= len)
            return;
        //选择父节点、所有孩子中最大的那个元素
        int hi = lo + Dchild - 1;
        int max = parent;
        for (int i = lo; i < len && i <= hi; i++) {
            if (arr[max] < arr[i])
                max = i;
        }
        //如果不是父节点，则交换，并对孩子节点接续做heapify
        if (parent != max) {
            swap(arr, parent, max);
            ShiftDownInDHeap(arr, max, len);
        }
    }


    void buildDHeap(T *arr, int len) {
        for (int i = (len - 1) / Dchild; i >= 0; i--)
            ShiftDownInDHeap(arr, i, len);
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

#endif //HEAPSORT_HPP