适配器本是一种设计模式，将一个类的功能进行包装成另外一种接口。在STL中提供的适配器，也是扮演着转换器的角色。

# adapters的分类

在STL中一共有6个组件，其中适配器可以和其中的三个组件进行适配，所以adapters就分成了三种类型，这三种类型分别是**仿函数适配器**、**容器适配器**、**迭代器适配器**。

# 容器适配器

容器适配器其实和设计模式中的适配器是一个概念，比如在STL中的stack和queue的实现方式就是一种容器适配器。我们来看下这来个容器的实现。

```cpp
template<typename _Tp, typename _Sequence = std::deque <_Tp> >
class queue {
    // ...
    _Sequence c;
    reference
    front() {
        __glibcxx_requires_nonempty();
        return c.front();
    }
    
    const_reference
    front() const {
        __glibcxx_requires_nonempty();
        return c.front();
    }
};
```

这里关于queue的所有操作底层都是通过调用deque来实现的，也就是说queue只是对deque提供的功能进行了转换。所以如果我们自己写一个容器，也能够提供queue所需要的接口，也是可以成为queue的底层支撑的。
在STL中的list双向链表也能够提供和deque相同的功能，所以也可以使用list作为底层支撑。

```cpp
   std::queue<int, std::list<int>> q;  // 这样声明也是对的
```

所以这里queue就是容器适配器的概念。将deque这个容器改头换面，让其拥有另外的功能，仅此而已。

statck的实现过程与此相同。

# 迭代器适配器

# 仿函数适配器

