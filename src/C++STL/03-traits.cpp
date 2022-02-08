#include <iostream>

using namespace std;
namespace template_argument_deduction {
    template<typename Type, typename Point, typename Ref>
    void fun_impl(Type t, Point p, Ref r) {
        cout << "value type : " << typeid(Type).name() << endl;
        cout << "point type : " << typeid(Point).name() << endl;
        cout << "reference type : " << typeid(Ref).name() << endl;
    }

    // 通过函数模板自动推到出值
    template<typename I>
    void test(I iter) {
        fun_impl(iter, &iter, (iter));
    }
}


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

int main(void) {
    double d = 0;
    template_argument_deduction::test(d);
    cout << "=============" << endl;

    Iterator<int> ite(new int(10));
    auto pointer = traits_return_type::return_pointer(ite);
    cout << "return_pointer " << typeid(pointer).name() << endl;

    auto reference = traits_return_type::return_reference_type(ite);
    cout << "return_reference_type " << typeid(reference).name() << endl;

    auto value_type = traits_return_type::return_value_type(ite);
    cout << "return_value_type " << typeid(value_type).name() << endl;

    cout << "=============" << endl;
}


