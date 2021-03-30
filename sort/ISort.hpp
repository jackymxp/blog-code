//
// Created by 孟祥鹏 on 2020/11/28.
//

#ifndef ISORT_H
#define ISORT_H

#include <cmath>

template<typename T>
class ISort {
public:

    virtual void sortIterative(T *arr, const int len) = 0;

    virtual void sortRecursive(T *arr, const int len) = 0;

    virtual void sortAdvanced(T *arr, const int len) = 0;


};


#endif //ISORT_H
