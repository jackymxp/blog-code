#ifndef SHELLSORT_HPP
#define SHELLSORT_HPP

template<typename T>
class ShellSort {

public:
    void sortIterative(T *arr, const int len) {
        shellSortIterative(arr, len);
    }

    void sortRecursive(T *arr, const int len) {
        shellSortRecursive(arr, len);
    }

    void sortAdvanced(T *arr, const int len) {
        shellSortAdvanced(arr, len);
    }

public:
    void shellSortIterative(T *arr, int len) {
        int step = 1;
        while (step < len / 3)
            step = step * 3 + 1;

        while (step >= 1) {
            //这里和插入排序一样，将1替换成step即可
            for (int i = step; i < len; i++) {
                for (int j = i; j >= step && (arr[j] < arr[j - step]); j -= step)
                    swap(arr, j, j - step);
            }
            step = step / 3;
        }
    }

    void shellSortRecursive(T *arr, int len) {
        int step = 1;
        while (step < len / 3)
            step = step * 3 + 1;
        shellSortRecursive(arr, len, step);
    }

    void shellSortRecursive(T *arr, int len, int step) {
        if (step < 1)
            return;

        for (int i = step; i < len; i++) {
            T tmp = arr[i];
            int j = i - step;
            while (j >= 0 && (tmp < arr[j])) {
                arr[j] = arr[j + step];
                j -= step;
            }
            arr[j + step] = tmp;
        }
        shellSortRecursive(arr, len, step / 3);
    }

    void shellSortAdvanced(T *arr, int len) {
        int step = 1;
        while (step < len / 3)
            step = step * 3 + 1;
        while (step >= 1) {
            for (int i = step; i < len; i++) {
                T tmp = arr[i];
                int j = i - step;
                while (j >= 0 && (tmp < arr[j])) {
                    arr[j] = arr[j + step];
                    j -= step;
                }
                arr[j + step] = tmp;
            }
            step = step / 3;
        }
    }

private:
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


#endif //SHELLSORT_HPP
