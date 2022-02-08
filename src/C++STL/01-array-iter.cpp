#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

template<typename T>
class Array {
private:
    static constexpr int len = 100;
    T m_data[len];
public:
    typedef T value_type;
    typedef value_type *pointer;
    typedef value_type &reference;

    int capacity() const {
        return len;
    }

    reference operator[](int i) {
        return m_data[i];
    }

    value_type operator[](int i) const {
        return m_data[i];
    }

    struct iterator : public std::iterator<std::random_access_iterator_tag, T> {
        explicit iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }

        pointer operator->() const { return m_ptr; }

        iterator &operator=(const iterator &iter) {
            m_ptr = iter.m_ptr;
            return *this;
        }

        bool operator!=(const iterator &iter) {
            return iter.m_ptr != m_ptr;
        }

        bool operator==(const iterator &iter) {
            return iter.m_ptr == m_ptr;
        }

        iterator &operator++() {
            m_ptr++;
            return *this;
        }

        iterator operator++(int) {
            iterator t = *this;
            ++(*this);
            return t;
        }

        friend bool operator==(const iterator &a, const iterator &b) { return a.m_ptr == b.m_ptr; };

        friend bool operator!=(const iterator &a, const iterator &b) { return a.m_ptr != b.m_ptr; };


    private:
        pointer m_ptr;
    };


    iterator begin() { return iterator(&m_data[0]); }

    iterator end() { return iterator(&m_data[0] + len); }
};

int main() {
    Array<double> arr{};
    double target = 10;
    for (int i = 0; i < arr.capacity(); i++) {
        arr[i] = target;
    }
    for (double &it : arr) {
        std::cout << it << " ";
    }
    std::cout << std::endl;
    std::cout << std::count_if(arr.begin(), arr.end(), [&target](int a) { return a == target; }) << std::endl;
}