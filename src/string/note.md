# 归并排序

回顾一下$$O(n^2)$$级别的排序算法，在冒泡排序、选择排序、插入排序中，视角都停留在了某一个元素的排序，也就是一次冒泡或插入操作只能将一个元素放在正确的位置，而一次操作的时间复杂度是$$O(n)$$，所以排序$$n$$个元素的时间复杂度就是$$O(n^2)$$。

这次换个角度，将目光聚焦到整个数组的排序上，而非某个元素的排序，可以这样考虑。

1. 将一个数组拆成两个部分。
2. 对前半段数组进行排序，对数组的后半段进行排序，这样前后半段就都是有序的了。至于如何排序前半段和后半段，可以用其他排序方法，当然也可以用归并排序啊。如果用归并，那就是递归喽。
3. 将有序的前半段数组和有序的后半段数组合并起来，就完成了整个数组的排序。

这就是归并排序的基本思想，归并排序和快速排序一样，都是**分治策略**的一种排序算法。

### 代码实现

假设排序前半段和后半段是采用快速排序进行排序的，我们就可以写出一个简单的代码框架

```cpp
//对数组arr[start...end]进行排序
void mergeSort(T* arr, const int start, int end){
    if(start >= end)
        return ;
    int mid = ((end - start) >> 1) + start;
    quickSort(arr, start, mid);
    quickSort(arr, mid+1, end);
    //合并两段有序数组arr[start...mid]和arr[mid+1, end]
    merge(arr, start, mid, end);
}
```

当然排序前半段和后半段也还可以用归并排序，这样这就是一个递归函数了。按照上面描述的三个步骤，来实现归并排序的代码框架。这样理解归并排序还可以？

```cpp
void mergeSort(T* arr, const int start, int end){
    if(start >= end)
        return ;
    int mid = ((end - start) >> 1) + start;
    mergeSort(arr, start, mid);    //排序前半段
    // assert(isSorted(arr, lo, mid));
    mergeSort(arr, mid+1, end);    //排序后半段
    // assert(isSorted(arr, mid+1, hi));
    merge(arr, start, mid, end);   //将有序的前半段和有序的后半段合并起来
}

void mergeSort(T* arr, const int len){
    mergeSort(arr, 0, len-1);
}
```

来一张图体会一下

![](https://cdn.jsdelivr.net/gh/jackymxp/image-bed/java/20201108115205.png)

框架写出来了，但是还剩下merge的代码没有实现了，思路很简单。可以参考[合并两个有序链表](https://leetcode-cn.com/problems/merge-two-sorted-lists/)或者[合并两个有序数组](https://leetcode-cn.com/problems/merge-sorted-array/)。[点击这里](https://leetcode-cn.com/problems/merge-sorted-array/)

数组的归并过程不能在同一个数组上完成，还需要额外使用一个数组保存原数组的副本。有了这个数据的副本后，merge的过程就非常简单了。

分别用一个下标`i`和`j`指向两段副本数组`aux[start...mid]`和`aux[mid+1...end]`的左端点，然后比较这两个下标的值，哪个小就将哪个值赋值给原数组，然后较小的下标向后移动一位，直到两个下标中有一个移动了数组的末尾，然后将另外一段数组直接赋值给原数组就可以了。

```cpp
void merge2Ways(T *arr, int start, int mid, int end) {
    T* aux = new T[end-start+1];
    //对arr[lo...mid]和arr[mid+1...hi] 归并
    for (int k = lo; k <= hi; k++)
        aux[k] = arr[k];
    //分别指向arr[lo...mid]和arr[mid+1...hi]的左侧端点
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) 
        arr[k++] = aux[i] < aux[j] ? aux[i++] : aux[j++];
    //还有一个没有赋值回去，直接赋值回去就行了
    while (i <= mid)
        arr[k++] = aux[i++];
    while (j <= hi)
        arr[k++] = aux[j++];
    delete []aux;
}
```

但是上面的`merge`合并过程并不好，因为在排序过程中要频繁的调用`merge`，每调用一次`merge`就需要为`aux`分配、释放一次内存。最好是可以一次就完成分配，这样就免去了频繁分配内存。所以在外部事先分配好内存供内部调用：

```cpp
void merge2Ways(T *arr, int start, int mid, int end, T* aux) {
    //对arr[lo...mid]和arr[mid+1...hi] 归并
    for (int k = lo; k <= hi; k++)
        aux[k] = arr[k];
    //分别指向arr[lo...mid]和arr[mid+1...hi]的左侧端点
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
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
```

为了方便观察排序过程，将排序的步骤打印出来。

```python
def mergeSort(arr):
    print("Splitting ",arr)
    if len(arr)>1:
        mid = len(arr)//2
        lefthalf = arr[:mid]
        righthalf = arr[mid:]

        mergeSort(lefthalf)
        mergeSort(righthalf)

        i=0
        j=0
        k=0
        while i < len(lefthalf) and j < len(righthalf):
            if lefthalf[i] <= righthalf[j]:
                arr[k]=lefthalf[i]
                i=i+1
            else:
                arr[k]=righthalf[j]
                j=j+1
            k=k+1

        while i < len(lefthalf):
            arr[k]=lefthalf[i]
            i=i+1
            k=k+1

        while j < len(righthalf):
            arr[k]=righthalf[j]
            j=j+1
            k=k+1
    print("Merging ",arr)

arr = [54,26,93,17,77,31,44,55,20]
mergeSort(arr)
print(arr)
```

输出结果

```text
Splitting  [54, 26, 93, 17, 77, 31, 44, 55, 20]
Splitting  [54, 26, 93, 17]
Splitting  [54, 26]
Splitting  [54]
Merging    [54]
Splitting  [26]
Merging    [26]
Merging    [26, 54]
Splitting  [93, 17]
Splitting  [93]
Merging    [93]
Splitting  [17]
Merging    [17]
Merging    [17, 93]
Merging    [17, 26, 54, 93]
Splitting  [77, 31, 44, 55, 20]
Splitting  [77, 31]
Splitting  [77]
Merging    [77]
Splitting  [31]
Merging    [31]
Merging    [31, 77]
Splitting  [44, 55, 20]
Splitting  [44]
Merging    [44]
Splitting  [55, 20]
Splitting  [55]
Merging    [55]
Splitting  [20]
Merging    [20]
Merging    [20, 55]
Merging    [20, 44, 55]
Merging    [20, 31, 44, 55, 77]
Merging    [17, 20, 26, 31, 44, 54, 55, 77, 93]
[17, 20, 26, 31, 44, 54, 55, 77, 93]
```

### 归并排序迭代

来看一下这个动图，这个动图演示的是使用迭代的归并排序。

![](https://cdn.jsdelivr.net/gh/jackymxp/image-bed/java/849589-20171015230557043-37375010.gif)

递归是自顶向下考虑排序过程，而迭代是自底向上思考。所以使用迭代实现首先考虑是只有1元素的数组归并成2个有序数组，然后再考虑2个元素的有序数组归并成4个元素有序数组。所以迭代代码的最外面一层循环应该是每次归并数组的规模，初始值为1的size，每次自增一倍，直到`size`的值和数组长度相等。

然后需要考虑的是如何`merge`，每次`merge`的时候左右端点值是什么？假设每次归并时左端点值为`lo`，每次归并`size`个元素，那么中间的下标应该是`lo+size-1`，这样数组`arr[lo…lo+size-1]`的元素个数为`size`个；那么最右侧端点应该是`lo+2*size-1`。这样每次可以归并的数组是`arr[lo…lo+2*size-1]`

最后一个问题，每次归并时最左侧端点值`lo`变量是怎么变化的，其实很简单，下一次需要归并的左端点和上一次归并的右端点不重合即可。因为一次归并的数组是`arr[lo…lo+(2*size-1)]`，下一次需要归并的是`arr[lo+2*size … lo+2*size+(2*size-1)]`。所以每次`lo`都应该自增`2*size`。另外因为`lo`表示最左侧的端点，结束的时候左侧端点距离数组尾端还有`size`个元素就可，所以`lo`的结束条件应该是`len-size`。

将其写成代码

```cpp
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
```

### 三路归并排序

在快速排序中，可以将数组分成二个部分，小于基准的和不小于基准的，改进的快排中，将数组拆分成是哪个部分，小于基准、等于基准和大于基准的；

在堆排序中，可以使用了二叉树构建堆，但是堆的性质中没有要求必须是二叉堆，所以在改进的堆排序中用d叉堆实现排序。

同样的，归并排序也不只是可以将数组固定的分成两个部分。也可以分成三个部分的，原理都是一样的。

假设mid1和mid2是原数组的三等分点。那么根据递归功能就有

1. 调用 `mergeSort3Way(arr, start, mid1, tmp)`完成前三分之一的排序；
2. 调用 `mergeSort3Way(arr, mid1+1, mid2, tmp)`完成中段三分之一的排序
3. 调用 `mergeSort3Way(arr, mid2+1, end, tmp)`完成后三分之一的排序；

最后将`arr[start…mid1]`和`arr[mid1+1...mid2]`与`arr[mid2+1…end]`三个有序数组归并即可，这里就是改一下`merge`的代码就可以完成三路归并排序，只不过这个`merge`过程稍微有一点复杂，但是代码逻辑很简单。

和快速排序一样，**最后能够改进的地方就是当数据量小到一定程度时候，切换到插入排序。**

```cpp
//合并三段有序数组arr[start, mid1]  arr[mid1+1, mid2], arr[mid2+1, end]
void merge3Ways(T *arr, int start, int mid1, int mid2, int end, T *aux) {
    for (int i = start; i <= end; i++)
        aux[i] = arr[i];
    int i = start, j = mid1 + 1, k = mid2 + 1, l = start;
    //三个值相互比较
    while ((i <= mid1) && (j <= mid2) && (k <= end)) {
        if (aux[i] < aux[j])
            arr[l++] = (aux[i] < aux[k]) ? aux[i++] : aux[k++];
        else
            arr[l++] = (aux[j] < aux[k]) ? aux[j++] : aux[k++];
    }
    //二个值相互比较
    while ((i <= mid1) && (j <= mid2))
        arr[l++] = (aux[i] < aux[j]) ? aux[i++] : aux[j++];
    while ((j <= mid2) && (k <= end))
        arr[l++] = (aux[j] < aux[k]) ? aux[j++] : aux[k++];
    while ((i <= mid1) && (k <= end))
        arr[l++] = (aux[i] < aux[k]) ? aux[i++] : aux[k++];
    //剩余一个直接填充
    while (i <= mid1)
        arr[l++] = aux[i++];
    while (j <= mid2)
        arr[l++] = aux[j++];
    while (k <= end)
        arr[l++] = aux[k++];
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

void mergeSort3Way(T *arr, int lo, int hi) {
    if (hi - lo <= 16) {
        insertSortAdvanced(arr, lo, hi);
        return;
    }
    T *aux = new T[hi - lo + 1];
    mergeSort3Way(arr, lo, hi, aux);
    delete[] aux;
}
```

这样很简单就完成了三路归并排序。貌似也没什么难度。

## 测试

针对1亿个随机数据集，下面是我电脑上的结果。

```java
MergeSort 迭代 21.6816 s, result = true
MergeSort 递归 22.0363 s, result = true
MergeSort 改进 20.4276 s, result = true
```

从结果表现上看，三路归并的结果要好于迭代和普通递归方案。

### 归并排序的应用

要分析归并排序的应用，首先要弄清楚归并排序的性能，这样才能更好的得出在什么场景下应用归并排序。

首先归并排序是一种稳定排序，并且其时间复杂度为$$O(nlogn)$$，从时间复杂度上来讲，归并排序是一种很优秀的算法，但是因为在排序中需要使用$$O(n)$$的空间，这导致在排序数组的时候，很少使用归并排序，尤其是在嵌入式设备上，因为他们的内存都相对比较珍贵。

一般在下面的几种情况下，使用归并排序。

1. 对于链表的排序，使用归并排序是最明智的选择。因为它不会耗费空间，且时间复杂度为$$O(nlogn)$$。
2. 找出数组中的逆序对的个数。
3. 在外部排序中应用归并排序。

### [链表的归并排序](https://leetcode-cn.com/problems/sort-list/)

```java
/**
 * Definition for singly-linked list.
 * public class ListNode {
 *     int val;
 *     ListNode next;
 *     ListNode() {}
 *     ListNode(int val) { this.val = val; }
 *     ListNode(int val, ListNode next) { this.val = val; this.next = next; }
 * }
 */
class Solution {
    private ListNode merge(ListNode p1, ListNode p2){
        if(p1 == null || p2 == null)
            return p1 == null ? p2 : p1;
        if(p1.val < p2.val){
            p1.next = merge(p1.next, p2);
            return p1;
        }
        else{
            p2.next = merge(p1, p2.next);
            return p2;
        }
    }
    private ListNode findMidNode(ListNode head){
        ListNode fast = head;
        ListNode slow = head;
        ListNode mid = head;
        while(fast != null && fast.next != null){
            fast = fast.next.next;
            mid = slow;
            slow = slow.next;
        }
        mid.next = null;
        return slow;
    }
    public ListNode sortList(ListNode head) {
        if(head == null || head.next == null)
            return head;
        //将链表拆成断，分别以head为头节点，和midNode;    
        ListNode midNode = findMidNode(head);
        ListNode p1 = sortList(head);
        ListNode p2 = sortList(midNode);
        return merge(p1, p2);
    }
}
```

### 完整代码

```cpp
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
```

