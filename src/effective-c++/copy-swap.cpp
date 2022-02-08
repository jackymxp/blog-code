#include <string>
#include <iostream>
#include <cstring>

using namespace std;


namespace str_move {
    class my_string {
    private:
        char *data;
        int len;
    public:

        explicit my_string(const char *s) : data(new char[strlen(s) + 1]), len(strlen(s)) {
            memcpy(data, s, len);
            data[len] = '\0';
            DCtor++;
        }

        // 拷贝构造函数
        my_string(const my_string &lv) : data(new char[strlen(lv.data) + 1]), len(lv.len) {
            memcpy(data, lv.data, len);
            data[len] = '\0';
            CCtor++;
        }

        my_string &operator=(const my_string &s) {
            if (&s == this)
                return *this;

            if (data) delete[] data;

            data = new char[strlen(s.data + 1)];
            memcpy(data, s.data, s.len);
            len = s.len;
            data[len] = '\0';
            CAsgn++;
            return *this;
        }

        // 移动构造函数
        my_string(my_string &&rv) noexcept : data(rv.data), len(rv.len) {
            rv.data = nullptr;  // 这里应该让原来的右值置为空。否则在析构函数上就会出现问题
            rv.len = 0;
            MCtor++;
        }

        my_string &operator=(my_string &&s) noexcept {
            if (&s == this)
                return *this;
            if (data) delete[] data;
            len = s.len;
            data = s.data;
            // 重要
            s.len = 0;
            s.data = nullptr;
            MAsgn++;
            return *this;
        }

        ~my_string() {
            if (data != nullptr)
                delete[] data;

            len = 0;
        }

        bool operator<(const my_string &s) {
            return string(this->data) < string(s.data);
        }

        static int DCtor;
        static int CCtor;
        static int CAsgn;
        static int MCtor;
        static int MAsgn;

        static void printStatic() {
            cout << "DCtor " << DCtor << endl;
            cout << "CCtor " << CCtor << endl;
            cout << "CAsgn " << CAsgn << endl;
            cout << "MCtor " << MCtor << endl;
            cout << "MAsgn " << MAsgn << endl;
        }
    };

    int my_string::DCtor = 0;
    int my_string::CCtor = 0;
    int my_string::CAsgn = 0;
    int my_string::MCtor = 0;
    int my_string::MAsgn = 0;
}

/*
 * 由于代码拷贝构造和赋值运算符代码中存在多处重复
 *
 * 存在自赋值安全问题
 *
 * 异常安全问题，比如new失败了，但是已经释放了原来的空间，导致回不到原来的样子了。
 *
 * 异常安全就是，改变这个变量就改成功了，如果改不成功，那就不要动。
 *
 * 解决方案使用copy-swap技术
 * */

namespace str_copy_swap {
    class my_string {
    private:
        char *data;
        int len;
    public:

        explicit my_string(const char *s = nullptr) : data(new char[strlen(s) + 1]), len(strlen(s)) {
            memcpy(data, s, len);
            data[len] = '\0';
            DCtor++;
        }

        // 拷贝构造函数
        my_string(const my_string &lv) : data(new char[strlen(lv.data) + 1]), len(lv.len) {
            memcpy(data, lv.data, len);
            data[len] = '\0';
            CCtor++;
        }

        my_string &operator=(my_string s) {  // 这里是值传递，也就是这个值不会要了
            swap(*this, s);
            CAsgn++;
            return *this;
        }

        // 移动构造函数
        my_string(my_string &&rv) noexcept : data(rv.data), len(rv.len) {
            rv.data = nullptr;  // 这里应该让原来的右值置为空。否则在析构函数上就会出现问题
            rv.len = 0;
            MCtor++;
        }

#if 0
        my_string &operator=(my_string &&s) noexcept {
            // 右值是将亡值，所以无情交换即可。
            swap(s, *this);
            MAsgn++;
            return *this;
        }
#endif

        ~my_string() {
            if (data != nullptr)
                delete[] data;

            len = 0;
        }

        // 定义一个swap
        // argument-dependent-lookup ADL
        friend void swap(my_string &lhs, my_string &rhs) noexcept {
            // 交换所有的成员变量
            std::swap(lhs.data, rhs.data);
            std::swap(lhs.len, rhs.len);
        }

        bool operator<(const my_string &s) {
            return string(this->data) < string(s.data);
        }

        static int DCtor;
        static int CCtor;
        static int CAsgn;
        static int MCtor;
        static int MAsgn;

        friend ostream &operator<<(ostream &os, const my_string &s);

        static void printStatic() {
            cout << "DCtor " << DCtor << endl;
            cout << "CCtor " << CCtor << endl;
            cout << "CAsgn " << CAsgn << endl;
            cout << "MCtor " << MCtor << endl;
            cout << "MAsgn " << MAsgn << endl;
        }
    };

    int my_string::DCtor = 0;
    int my_string::CCtor = 0;
    int my_string::CAsgn = 0;
    int my_string::MCtor = 0;
    int my_string::MAsgn = 0;

    ostream &operator<<(ostream &os, const my_string &s) {
        os << "len = " << s.len << " data = " << s.data;
        return os;
    }
}


/*
 * 上面的实现方式有几个弊端
 *
 * 1. 如果其他人使用这个类，怎么能知道这个类定义了swap呢？
 * 2. 如果我们没有定义swap，那么就需要让调用这使用std::swap版本，如果我们定义了，就调用我们自己写的。

    如果我这样去使用，就会报错，因为这里会产生歧义，= 不知道改用哪个版本。不知道用移动版本的还是普通版本。
 * */
namespace str_copy_swap_better {
    class my_string {
    private:
        char *data;
        int len;
    public:

        explicit my_string(const char *s = nullptr) : data(new char[strlen(s) + 1]), len(strlen(s)) {
            memcpy(data, s, len);
            data[len] = '\0';
            DCtor++;
        }

        // 拷贝构造函数
        my_string(const my_string &lv) : data(new char[strlen(lv.data) + 1]), len(lv.len) {
            memcpy(data, lv.data, len);
            data[len] = '\0';
            CCtor++;
        }

        my_string &operator=(my_string s) {  // 这里是值传递，也就是这个值不会要了
            swap(*this, s);
            CAsgn++;
            return *this;
        }

        // 移动构造函数
        my_string(my_string &&rv) noexcept : data(rv.data), len(rv.len) {
            rv.data = nullptr;  // 这里应该让原来的右值置为空。否则在析构函数上就会出现问题
            rv.len = 0;
            MCtor++;
        }

#if 0
        //my_string s1("abc");
        //s1 = my_string("123"); 这里string会产生歧义，所以取消这个版本即可。编译器会自动知道使用的是哪个版本。
        my_string &operator=(my_string &&s) noexcept {
            // 右值是将亡值，所以无情交换即可。
            swap(s, *this);
            MAsgn++;
            return *this;
        }
#endif

        ~my_string() {
            if (data != nullptr)
                delete[] data;

            len = 0;
        }

        // 定义一个swap
        // argument-dependent-lookup ADL
        friend void swap(my_string &lhs, my_string &rhs) noexcept {
            // 同样的，如果成员变量定义了swap，则使用成员变量的swap
            // 否则就使用std的。
            using std::swap;
            swap(lhs.data, rhs.data);
            swap(lhs.len, rhs.len);
        }

        bool operator<(const my_string &s) {
            return string(this->data) < string(s.data);
        }

        static int DCtor;
        static int CCtor;
        static int CAsgn;
        static int MCtor;
        static int MAsgn;

        friend ostream &operator<<(ostream &os, const my_string &s);

        static void printStatic() {
            cout << "DCtor " << DCtor << endl;
            cout << "CCtor " << CCtor << endl;
            cout << "CAsgn " << CAsgn << endl;
            cout << "MCtor " << MCtor << endl;
            cout << "MAsgn " << MAsgn << endl;
        }
    };

    int my_string::DCtor = 0;
    int my_string::CCtor = 0;
    int my_string::CAsgn = 0;
    int my_string::MCtor = 0;
    int my_string::MAsgn = 0;

    ostream &operator<<(ostream &os, const my_string &s) {
        os << "len = " << s.len << " data = " << s.data;
        return os;
    }

    class Bitmap{
    public:
        my_string ms;

        friend void swap(Bitmap& lhs, Bitmap& rhs) {
            using std::swap;
            // 如果my_string定义了，就使用my_string自己定义的更好的版本。
            // 如果my_string没有定义，那么就使用标准库提供的。
            swap(lhs.ms, rhs.ms);

            // 其他成员变量的swap
        }
    };
}


int main(void) {
    using str_copy_swap::my_string;

    my_string s1("abc");
    my_string s2 = s1;

    my_string s3 = my_string("def");
    cout << s1 << endl;
    cout << s2 << endl;
    cout << s3 << endl;

    using std::swap;

    swap(s1, s3);

    cout << s1 << endl;
    cout << s2 << endl;
    cout << s3 << endl;

    s1 = my_string("dfdf");
    cout << s1 << endl;
}