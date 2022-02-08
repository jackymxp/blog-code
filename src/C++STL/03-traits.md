# traits 类型萃取机

迭代器是算法和容器之间的桥梁，算法是通过迭代器来操作容器的内容的。

迭代器应该具有遍历容器内所有元素的能力，且行为是类似指针，通过这个指针，算法就能够操作容器中的元素内容。所以迭代器的一个最重要的工作就是让其具有指针和引用的行为。

在编程手法上应该具有重载`*`和`->`这两个函数。

迭代器想要操作元素，有时候就不可避免的想要获取一个元素的值类型，引用类型、指针类型。那么迭代器如何获取到容器内的元素类型的这些type呢？

答案就是通过萃取机。在谈论萃取机之前，先来看下在C++中有哪些操作可以获取到一个元素的类型。


# 利用函数模板来推导参数类型

可以通过函数模板的在编译时期的类型推导做到类型推导。这个比较简单，我们举个例子看下。


```cpp

```

不同的编译器上，针对typeid有不同的实现，但是从输出结果上我们大体可以看出，这是针指，还是值类型，甚至是引用类型。

虽然使用函数模板可以达到参数类型的推导的目的，但是针对返回值类型就束手无策了，那么有没有什么办法让编译器推导出返回值类型呢？

# 利用内嵌类型推导返回值类型

解决了函数参数模板的问题，来使用内嵌类型推导机制来推导返回值类型。直接写个demo。

```cpp

```

> 注意：这里的

这里从输出结果可以看到，使用内嵌类型是可以推导出返回值类型的，但是是有一个前提条件，就是传入的类型必须是一个class type，那么针对原生的类型，比如原生指针，这种要如何进行
类型推导呢？

答案是使用模板的偏特化来推导原生指针类型。

# 利用模板的偏特化推导原生指针类型

利用模板的偏特化，可以完成指针的萃取。这是因为偏特化，可以在模板参数上做特殊定制，这种定制包括

- 模板参数个数的定制
- 模板参数类型的定制

这里用到的是模板参数类型的偏特化，就是说当模板参数为指针类型的时候，不再使用原来的模板函数，而是使用特别针对指针的版本。

看下demo

```cpp

```

这里不仅为`int*`类型的做了特化，还针对了`const int*`版本的做了特化，如果传递进去的是`const int*`，由于类型是`const T*`，所以这里萃取出来T就是int。

这样就达到了不管是传递原始类型，就使用原始类型就行萃取；
传递的是`int*`这种指针类型，也可以萃取到原始类型int；当然这种也包括了`const int*`的指针版本。


# 定义引用和指针类型

由于使用模板的偏特化可以萃取到类型的原型，为了方便使用，这里多增加了引用类型和指针类型，都是再原生类型的基础上增加即可。看下代码。

```cpp

```

# 萃取类型的其他特性

## difference_type

由于一些算法的需求，比如lower_bound这些算法，需要让迭代器提供，两个迭代器之间的距离。所以就使用了difference_type来表示迭代器的距离。


# iterator_category

由于容器不同，所以元素在内存中的排布也不同，所以这就导致了迭代器具有不同的表现形式。
比如vector的迭代器具有随机访问的能力，即向前向后一次移动n个单位；
但是对于list这种容器的迭代器，一次只能移动一个单位，不具有一次移动多个单位的能力；
甚至还有一些迭代器只能单向移动；

针对这些迭代器所提供的能力，以及标准库对迭代器的使用要求，对迭代器分5类：

关于这个的范例，我们可以参考stl的advance的做法，advance的作用是将传递进去的迭代器前进n个单位。

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


template<typename _InputIterator, typename _Distance>
inline void
advance(_InputIterator &__i, _Distance __n) {
    // concept requirements -- taken care of in __advance
    typename iterator_traits<_InputIterator>::difference_type __d = __n;
    std::__advance(__i, __d, std::__iterator_category(__i));
}
```

这里使用第三个参数来区分调用不同的重载函数，而第三个参数的类型正好是iterator_categogy()，使用这个生成个临时对象，从而达到调用不同重载版本的目的。

