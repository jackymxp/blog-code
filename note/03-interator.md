# 为什么需要迭代器

在STL的设计中，大量用到了GP模板编程这种思想，不同于传统的OOP编程方法，GP模板编程思想是将**数据**和**算法**分开来设计的，而OOP编程思想是要将**数据**和**算法**融合到一起的设计思想。

比如标准库中的sort方法，可以对线性容器vector排序，也可以对array这种容器进行排序。这样在设计的时候，容器和算法就可以分开来设计了，从而达到**数据和算法之间的解耦**。

但是也会有例外，比如list容器，它的底层是双向链表，而标准库提供的sort版本是使用快速排序的，对链表排序速度很慢，所以针对链表会有自己的sort归并排序版本，除此之外，在list中还提供了merge这种算法，这样将算法融合到容器中，也即是封装数据，就是OOP的编程思想。

既然标准库大部分是采用了OOP的编程理念，那么算法是什么来操作容器内的元素的呢？答案就是通过迭代器，

STL中有很多已经实现好了的算法，如果我们自己实现了某种容器，想要借用STL中的算法，那么就需要通过迭代器这个桥梁来作为容器和算法之间的迭代器。所以我们还需要为我们自己实现的容器设计一个迭代器。

# 迭代器应该有什么能力

迭代器作为算法和容器之间的桥梁，算法是通过迭代器来操作容器中的元素的。这样在传递到算法的时候，通常传递一个区间即可。这个区间是左闭右开的，这种有几种好处：

- 空集的概念很好表示，首尾相同即可 [beg, beg)
- 比较容易返回错误值，数据查找，如果没有找到，我们不需要返回一个特殊值，直接返回`end`，就可以来，因为`end`不在区间内部，返回它很好的表达没有找到这个概念。
- 比较容易表达迭代终止条件这个概念，`beg == end`即可表示迭代终止，这对于迭代器来说是很重要的，因为它不需要支持算数操作符，只需要支持判等符即可。

这里提到的算法的输入是两个迭代器，这里的迭代器主要提供两个功能，标识元素以及遍历容器。对于一个普通的泛型算法来说，遍历功能表现在它的参数上面，而标识功能表现在它的返回值。这两个功能的存在意味着所有的迭代器都支持下面两个操作：

- `*i`解引用，因为迭代器支持标识元素，解引用可能是用于取值也可能是用于赋值，具体看后文迭代器的分类。
- `++i`，因为迭代器支持遍历。

那么重载`*`的时候，需要让迭代器返回的是引用类型；如果重载了`->`，需要让迭代器具有指针的行为，也就是需要返回一个指针类型。有时候通过迭代器访问，还需要返回值类型，那么问题来了，通过迭代器，我们怎么能够获取到元素的reference、pointer或者是value_type呢？


答案就是通过萃取机。在谈论萃取机之前，先来看下在C++中有哪些操作可以获取到一个元素的类型。


# 利用函数模板来推导参数类型

可以通过函数模板的在编译时期的类型推导做到类型推导。这个比较简单，举个例子看下。


```cpp
template<typename Type, typename Point, typename Ref>
void fun_impl(Type t, Point p, Ref r) {
    cout << "value type : " << typeid(Type).name() << endl;       // value type : d
    cout << "point type : " << typeid(Point).name() << endl;      // point type : Pd
    cout << "reference type : " << typeid(Ref).name() << endl;    // reference type : d
}

// 通过函数模板自动推到出值
template<typename I>
void test(I iter) {
    fun_impl(iter, &iter, (iter));
}

int main() {
    double d = 0;
    template_argument_deduction::test(d);
}

```

不同的编译器上，针对typeid有不同的实现，但是从输出结果上我们大体可以看出，这是针指，还是值类型，或者是引用类型。

虽然使用函数模板可以达到参数类型的推导的目的，但是针对返回值类型就束手无策了，那么有没有什么办法让编译器推导出返回值类型呢？

# 利用内嵌类型推导返回值类型

使用函数模板推到机制，可以解决参数传递的问题，那么使用内嵌类型推导机制来推导返回值类型。

```cpp
template<class T>
struct Iterator {
    typedef T value_type;  // 内嵌类型
    typedef T &reference;  // 内嵌类型
    typedef T *pointer;  // 内嵌类型
    T *ptr;
    explicit Iterator(T *p = 0) : ptr(p) {}
    T &operator*() const { return *ptr; };
};


// 返回值类型不能推导，所以使用内嵌类型，但是不能推到原生类型
namespace traits_return_type {
    template<class I>
    // 这里typename不能少，要明确告诉编译器这是一个类型，不是static成员变量或函数
    typename I::pointer return_pointer(I iter) {
        return &(*iter);
    }

    template<class I>
    typename I::value_type return_value_type(I iter) {
        return *iter;
    }

    template<class I>
    typename I::reference return_reference_type(I iter) {
        return *iter;
    }
}

int main() {
    Iterator<int> ite(new int(10));

    auto pointer = traits_return_type::return_pointer(ite);
    cout << "return_pointer " << typeid(pointer).name() << endl;  // return_pointer Pi

    auto reference = traits_return_type::return_reference_type(ite);
    cout << "return_reference_type " << typeid(reference).name() << endl;  // return_reference_type i

    auto value_type = traits_return_type::return_value_type(ite);
    cout << "return_value_type " << typeid(value_type).name() << endl;   // return_value_type i
}
```

这里从输出结果可以看到，使用内嵌类型是可以推导出返回值类型的，但是是有一个前提条件，就是传入的类型必须是一个class type，那么针对原生的类型，比如原生指针，例如`int*`，或者是`const int*`类型，这种要如何进行
类型推导呢？

答案是使用模板的偏特化来推导原生指针类型。

# 利用模板的偏特化推导原生指针类型

利用模板的偏特化，可以完成指针的萃取。这是因为偏特化，可以在模板参数上做特殊定制，这种定制包括

- 模板参数个数的定制
- 模板参数类型的定制

这里用到的是模板参数类型的偏特化，就是说当模板参数为指针类型的时候，不再使用原来的模板函数，而是使用特别针对指针的版本。

```cpp

// 上面的只能推导class type，不能推导原生指针，用模板偏特化萃取类型
namespace traits_point {
    // 传递原生类型，就用这个traits
    template<typename I>
    struct iter_traits {
        iter_traits() {
            cout << "iter_traits" << endl;
        }

        // 如果类型I定义了一个value_type，那么就取出这个类型，然后使用typedef 重新命名为value_type;
        typedef typename I::value_type value_type;

        // 除了原生类型，还要traits 指针、引用类型
        typedef typename I::pointer pointer;
        typedef typename I::reference reference;
    };

    // 如果传递的是指针类型，就用这个iter_traits
    template<typename I>
    struct iter_traits<I *> {
        iter_traits() {
            cout << "iter_traits<I *>" << endl;
        }

        // 萃取值类型
        typedef I value_type;

        // 除了原生类型，还要traits 指针、引用类型
        typedef I *pointer;
        typedef I &reference;
    };

    // 如果传递的是 const 指针类型，就用这个iter_traits
    template<typename I>
    struct iter_traits<const I *> {
        iter_traits() {
            cout << "iter_traits<const I *>" << endl;
        }

        // 萃取值类型
        typedef I value_type;

        // 除了原生类型，还要traits 指针、引用类型
        typedef I *pointer;
        typedef I &reference;
    };

    void test() {
        iter_traits<const int *>();  // iter_traits<const I *>
        iter_traits<int *>();        // iter_traits<I *>
        iter_traits<int *>::value_type v;
        iter_traits<int *>::reference r = v;
        iter_traits<int *>::pointer p = &v;
        cout << "value_type " << typeid(v).name() << endl;   // value_type i
        cout << "reference_type " << typeid(r).name() << endl;  // reference_type i
        cout << "pointer_type " << typeid(p).name() << endl;  // pointer_type Pi
    }
}
```

这里不仅为`int*`类型的做了特化，还针对了`const int*`版本的做了特化，如果传递进去的是`const int*`，由于类型是`const T*`，所以这里萃取出来T就是int。这里就实现了不管传递的是什么类型，都能够实现原始类型的萃取。然后根据原始类型，实现了指针类型和引用类型。


# 萃取类型的其他特性

## difference_type

由于一些算法的需求，比如lower_bound这些算法，需要让迭代器提供，两个迭代器之间的距离。所以就使用了difference_type来表示迭代器的距离。这个函数的签名是distance，传入的是两个迭代器指针。





## iterator_category

由于容器特征不同，所以容器内的元素在内存中的排布也不同，有连续使用内存空间的，也有非连续使用内存的，这就导致了迭代器在移动的时候，具有不同的性质。
比如vector的底层是使用数组，在内存空间中是连续的，所以vector的迭代器具有随机访问的能力，即向前向后一次移动n个单位；
但是对于list，底层是使用双向链表作为支撑的，所以list的迭代器，一次只能移动一个单位，不具有一次移动多个单位的能力；
而有些容器，如forward_list底层是使用单向链表，所以它的迭代器只是具有了单向移动的能力。

针对这些迭代器所提供不同的能力，以及标准库对迭代器的使用要求，对迭代器分5类：

|      | 迭代器类型                                                   | 描述                                                     |
| ---- | :----------------------------------------------------------- | :------------------------------------------------------- |
| 1    | **[输入迭代器](https://en.cppreference.com/w/cpp/named_req/InputIterator)** | 只能向前扫描容器一次，不能改变它指向的值（只读）；       |
| 2    | **[输出迭代器](https://en.cppreference.com/w/cpp/named_req/OutputIterator)** | 只能向前扫描容器一次，不能读取它指向的值（只写）；       |
| 3    | **[前向迭代器](https://en.cppreference.com/w/cpp/named_req/ForwardIterator)** | 可以多次向前扫描容器，可以读写它指向的值；               |
| 4    | **[双向迭代器](https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator)** | 与上一个相同，但可以来回扫描容器；                       |
| 5    | **[随机访问迭代器](https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator)** | 与前一个相同，但也可以非顺序访问容器（即通过跳跃）；     |
| 6    | **[连续迭代器](https://en.cppreference.com/w/cpp/named_req/ContiguousIterator)** | 与前一个相同，除了逻辑上相邻的元素在内存中也物理上相邻。 |


有了这些类别，所以我们在使用一些函数的时候，这些函数就可以根据迭代器的类型，而采用效率更高的做法，比如stl的advance的做法，advance的作用是将传递进去的迭代器前进n个单位。

这个在针对不同的迭代器，就采用了不同的能力，如果传递的是随机访问迭代器，那么具有跳跃功能，可以一次增加n个单位。
如果传递的是连续迭代器，那么就只能一次移动一个单位的距离。所以先针对这三种不同类型的迭代器，来写出不同的版本。

```cpp

template<typename _InputIterator, typename _Distance>
inline void
__advance(_InputIterator &__i, _Distance __n, input_iterator_tag) {
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept < _InputIterator >)
    __glibcxx_assert(__n >= 0);
    while (__n--)
        ++__i;
}

template<typename _BidirectionalIterator, typename _Distance>
inline void
__advance(_BidirectionalIterator &__i, _Distance __n,
          bidirectional_iterator_tag) {
    // concept requirements
    __glibcxx_function_requires(_BidirectionalIteratorConcept <
                                _BidirectionalIterator >)
    if (__n > 0)
        while (__n--)
            ++__i;
    else
        while (__n++)
            --__i;
}

template<typename _RandomAccessIterator, typename _Distance>
inline void
__advance(_RandomAccessIterator &__i, _Distance __n,
          random_access_iterator_tag) {
    // concept requirements
    __glibcxx_function_requires(_RandomAccessIteratorConcept <
                                _RandomAccessIterator >)
    if (__builtin_constant_p(__n) && __n == 1)
        ++__i;
    else if (__builtin_constant_p(__n) && __n == -1)
        --__i;
    else
        __i += __n;
}
```

最后在使用的时候，应该提供一个统一的接口给调用者去用，然后由这个函数去选择那个更好的版本即可，那么在这个统一的函数中，如果区分应该调用哪个函数呢？

这里iterator_categogy 这个参数就派上用场了。

```cpp

template<typename _InputIterator, typename _Distance>
inline void
advance(_InputIterator &__i, _Distance __n) {
    // concept requirements -- taken care of in __advance
    typename iterator_traits<_InputIterator>::difference_type __d = __n;
    std::__advance(__i, __d, std::__iterator_category(__i));
}
```

这里使用第三个参数来区分调用不同的重载函数，而第三个参数的类型正好是 iterator_categogy()，使用这个生成个临时对象，利用函数模板推导机制，来达到调用不同重载版本的目的。

这个就是iterator_category的作用。


# 手写一个迭代器

既然我们知道，一个迭代器要提供这么多，那么承认打铁，手写一个支持range-for循环的容器，使用迭代器来遍历其中的元素。

```cpp



```



参考
http://blog.guorongfei.com/2018/04/13/algorithm-and-iterator/
