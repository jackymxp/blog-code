#include <iostream>
#include <ctime>
#include "rbtree.hpp"
#include "avl.hpp"
#include "bst.hpp"
#include <thread>

#ifndef NDEBUG
#   define ASSERT(condition, message) \
do { \
if (! (condition)) { \
std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
<< " line " << __LINE__ << ": " << message << std::endl; \
std::terminate(); \
} \
} while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

template<typename TREE>
bool testFunction(const char *name, TREE &tree, const int len, int step = 4) {
    printf("test %s\n", name);
    auto s = clock();
    bool ret = true;
    for (int i = 0; i < len; i++) {
        tree.add(i, i);
        if (!tree.check()) {
            printf("add check is error\n");
            ret = false;
        }
    }
    if (tree.size() != len) {
        printf("add size is error\n");
        ret = false;
    }

    for (int i = 0; i < len; i++) {
        int r = tree.rank(i);
        if (r != i) {
            printf("rank is error\n");
            ret = false;
        }
    }

    for (int i = 0; i < len; i += step) {
        tree.remove(i);
        if (!tree.check()) {
            std::cout << "remove check error " << std::endl;
            ret = false;
        }
    }

    if (tree.size() != len - len / step - (len % step == 0 ? 0 : 1)) {
        printf("error size in remove and add\n");
        ret = false;
    }

    for (int i = 0; i < len; i++) {
        const int *a = tree.get(i);
        if (i % step == 0) {
            if (a != nullptr) {
                printf("get() is error\n");
                ret = false;
            }
        } else {
            if (*a != i) {
                printf("get() is error\n");
                ret = false;
            }
        }
    }

    for (int i = 0; i < len; i++) {
        bool c = tree.contains(i);
        if (i % step == 0) {
            if (c) {
                printf("contains is error\n");
                ret = false;
            }
        } else {
            if (!c) {
                printf("contains is error\n");
            }
        }
    }

    for (int i = 0; i < len; i++) {
        tree.remove(i);
        if (!tree.check()) {
            printf("remove error \n");
            ret = false;
        }
    }

    if (tree.size() != 0) {
        printf("error remove and add\n");
        ret = false;
    }

    if (!tree.empty()) {
        printf("empty() is error\n");
        ret = false;
    }

    for (int i = 0; i < len; i++) {
        tree.add(i, i);
    }
    int k = 0;
    while (!tree.empty()) {
        tree.removeMin();
        if (tree.size() != len - k - 1) {
            printf("removeMin size is error\n");
            ret = false;
        }
        const int *m = tree.min();
        if (m && *m != ++k) {
            printf("removeMin min is error\n");
            ret = false;
        }
        if (!tree.check()) {
            printf("removeMin check is error\n");
            ret = false;
        }
    }


    for (int i = 1; i <= len; i++) {
        tree.add(i, i);
        if (tree.size() != i) {
            printf("add is error, should %d, real %d\n", i, tree.size());
            ret = false;
        }
    }

    k = 0;

    while (!tree.empty()) {
        tree.removeMax();
        if (tree.size() != len - k - 1) {
            printf("removeMax size is error, should %d, real %d, k = %d, empty: %d\n", len - k - 1, tree.size(), k,
                   tree.empty());
            ret = false;
        }
        const int *m = tree.max();
        if (m && *m != len - k - 1) {
            printf("removeMax max is error, should %d, real %d\n", len - k - 1, *m);
            ret = false;
        }
        if (!tree.check()) {
            printf("removeMax check is error\n");
            ret = false;
        }
        k++;
    }

    for (int i = 0; i < len; i += step) {
        tree.add(i, i);
    }

    const int *c = tree.ceiling(2);
    if (c == nullptr || *c != step) {
        printf("ceiling() is error, should %d, real %d\n", step, *c);
        ret = false;
    }

    const int *f = tree.floor(2);
    if (f == nullptr || *f != 0) {
        printf("floor() is error, should %d, real %d\n", 0, *f);
        ret = false;
    }

    printf("%s height: %d\n", name, tree.height());
    auto e = clock();
    printf("test %s spent %ld\n", name, e - s);
    return ret;
}


template<typename TREE>
clock_t testPerformance(TREE &tree, const int len, const int step = 4) {
    clock_t s = clock();
    // 增加
    for (int i = 0; i < len; i++) {
        tree.add(i, i);
    }
    // 查找
    for (int i = len; i >= 0; i--) {
        tree.get(i);
    }

//     删除
    for (int i = 0; i < len; i += step) {
        tree.remove(i);
    }

    for (int i = 0; i < len; i += step) {
        tree.ceiling(i);
    }

    for (int i = 0; i < len; i += step) {
        tree.floor(i);
    }

    for (int i = 0; i < len; i += step) {
        tree.floor(i);
    }
    clock_t e = clock();
    clock_t d = e - s;
    return d;
}


template<typename TREE>
void testPerformanceTime(const char *name, TREE &tree, const int len, int step = 4) {
    auto t = testPerformance(tree, len, step);
    printf("%s spent %ld\n", name, t);
}

int main() {
    BSTree<int, int> bst;
    AVLTree<int, int> avl;
    RBTree<int, int> rbt;
    int len = 2000;

    std::thread btt([&]() { testFunction("bst", bst, len); });
    std::thread att([&]() { testFunction("avl", avl, len); });
    std::thread rtt([&]() { testFunction("rbt", rbt, len); });

    btt.join();
    att.join();
    rtt.join();

    len = 200000;
    int step = 4;
    std::thread bt([&]() { testPerformanceTime("avl", avl, len, step); });
    std::thread at([&]() { testPerformanceTime("rbt", rbt, len, step); });
    std::thread rt([&]() { testPerformanceTime("bst", bst, len, step); });

    bt.join();
    at.join();
    rt.join();
    return 0;
}