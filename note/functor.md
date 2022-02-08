在STL的使用过程中，似乎只有仿函数是具有我们灵魂的代码，因为仿函数是和算法配套的组件，用来控制算法的执行策略。

# 函数对象

在STL提供的算法中，一般都会提供两个版本，这两个版本之间构成重载的关系，比如：

```cpp
unique(begin, end);   // 使用 == 运算符
unique(begin, end, comp);  // 使用 comp 运算符
```

默认情况下，这些算法使用<和==运算符完成比较。另外一个重载版本的第三参数是允许我们按照自定义的操作来代替默认操作，从而达到我们期望的行为。那么也就是说这个过程需要向算法传递一个函数。这个参数就是仿函数，也叫函数对象，看下在C++中都有哪些是仿函数。


## 重载函数调用符的类是函数对象

如果一个类重载了函数调用符，这个对象称为仿函数，它在功能上是一个函数，在语法上是一个类。它和普通函数最大的区别是它可以保存内部状态，所以与普通函数相比它们更加灵活。

例如这个类，就可以称为函数对象。

```cpp

```

除此之外，函数调用符还可以进行重载。

当然也可以像一个正常的类使用函数调用符，用来保存一些内部状态。如下例子，使用`cout`打印字符串的内容。


```cpp
class PrintString {
public:
    PrintString(std::ostream &os = std::cout, char c = '\n') : os(os), sep(c) {
    }

    void operator()(const std::string &s) const { os << s << sep; }

    char sep;
    std::ostream &os;
};    

std::string s{"hello world"};  
PrintString()(s); // hello world

std::vector<std::string> vs {"hello", "world", "C++"};
std::for_each(vs.begin(), vs.end(), PrintString(std::cerr ));
```


## lambda表达式是函数对象


在C++中，除了重载调用运算符的类之外，lambda构造的也是一个匿名的函数对象，比如：

```cpp
[&num](int a) {
    return a % num;
}
```

实际上等同于

```cpp
class ClosureType {
public:
    ClosureType(int& num) : num_(num) {}

    auto operator()(int a) const {
        return a % num_;
    }
private:
    int& num_;
};
```

有关lambda的更多[参考](https://devdocs.io/cpp/language/lambda)

## function大一统

C++语言中有几种可调用的对象：

- 函数
- 函数指针
- lambda表达式
- bind创建的对象
- 重载了函数调用运算符的类。

在这些可调用的对象的使用上，是具有相同的调用形式，比如。

```cpp
int add(int i, int j){return i + j;}

auto mod = [](int i, int j) {return i % j;};

struct divide {
    int operator() (int i, int j) {return i / j;}
};
```

这些可调用对象都有同一种调用形式，即传入的是两个int类型，返回的也是int类型，那么他们理应有相同的type，在C++中，提供了一种高级用法function。它是一个模板，完成了对这些可i盗用对象的封装。

比如我们用function将上述的形式都进行封装下。

```cpp
    std::function<int(int, int)> f1 = add;
    std::function<int(int, int)> f2 = mod;
    std::function<int(int, int)> f3 = divide();
    
    std::cout << f1(1,32) << std::endl;  // 33
    std::cout << f2(32,3) << std::endl;  // 2
    std::cout << f3(32,3) << std::endl;  // 10

```

如果你愿意，你还可以这样做。

```cpp

std::map<char, std::function<int(int, int)>> mapping{{'+', add}, {'%', mod}, {'/', divide()}};

std::cout << mapping['+'](10, 2) << std::endl;  // 12
std::cout << mapping['%'](10, 2) << std::endl;  // 0
std::cout << mapping['/'](10, 2) << std::endl;  // 5

```

## 标准库定义的函数对象

[参考](https://en.cppreference.com/w/cpp/utility/functional)

# 自定义的仿函数如何接入到算法中

和迭代器一样，如果我们自定义的迭代器要能够被STL所使用，需要在自定义的迭代器中定义5中type，或者是继承来自STL提供的类。那么如果我们自定义的仿函数如果要很好的和STL一起工作，也是需要满足一定的条件的。

在STL中，仍然是提供类供我们继承，其中unary_function用一元函数的形式，定义了参数类型以及返回值型别。定义如下：

```cpp
template<typename _Arg, typename _Result>
struct unary_function
{
    /// @c argument_type is the type of the argument
    typedef _Arg 	argument_type;   

    /// @c result_type is the return type
    typedef _Result 	result_type;  
};
```

在STL中，有一些一斤实现了的仿函数来继承unary_function，比如。

```cpp

template<class Pair>
struct select2nd : public std::unary_function<Pair, typename Pair::second_type> {
    const typename Pair::second_type &operator()(const Pair &x) const {
        return x.second;
    }
};

std::pair<int, double> p = {1, 3.14};
auto k = select2nd<std::pair<int, double>>()(p);
std::cout << k << std::endl;  // 3.14

```
这个仿函数继承了unary_function，其中利用second_type提取出来了第二个参数类型。这就是标准库中的做法。

如果是二元函数，那么也提供了对应的类让我们继承。

```cpp
template<typename _Arg1, typename _Arg2, typename _Result>
struct binary_function
{
    /// @c first_argument_type is the type of the first argument
    typedef _Arg1 	first_argument_type; 

    /// @c second_argument_type is the type of the second argument
    typedef _Arg2 	second_argument_type;

    /// @c result_type is the return type
    typedef _Result 	result_type;
};
```

这样我们在用的时候，就可以定义泛型的plus了。

```cpp
template<typename T>
struct plus : public std::binary_function<T, T, T> {
    T operator()(const T &x, const T &y) const { return x + y; }
};
```

实际上，标准库对plus也是这样进行定义的。
