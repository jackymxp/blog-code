#include <iostream>
#include <vector>
#include <cassert>

class quick_find {
public:
    explicit quick_find(const int count) : count(count), size(count), id(new int[count]) {
        for (int i = 0; i < count; i++)
            id[i] = i;
    }

    void union_operator(int p, int q)  {
        int qId = find(q);
        int pId = find(p);
        if (qId == pId)
            return;
        for (int i = 0; i < count; i++) {
            if (id[i] == qId)
                id[i] = pId;
        }
        size--;
    }


    bool connected(int p, int q)  {
        return find(p) == find(q);
    }

    ~quick_find() {
        delete[] id;
    }

private:
    int find(int p) {
        assert(p >= 0 && p < count);
        return id[p];
    }

private:
    int count;
    int size;
    int *id;
};

class quick_union {
public:
    explicit quick_union(const int count) : count(count), size(count), id(new int[count]) {
        for (int i = 0; i < count; i++)
            id[i] = i;
    }

    void union_operator(int p, int q)  {
        int qId = find(q);
        int pId = find(p);
        if (qId == pId)
            return;

        id[pId] = qId;
        size--;
    }

    bool connected(int p, int q)  {
        return find(p) == find(q);
    }

    ~quick_union() {
        delete[] id;
    }

private:
    int find(int p) {
        assert(p >= 0 && p < count);
        while (p != id[p])
            p = id[p];
        return p;
    }

private:
    int count;
    int size;
    int *id;
};

class weight_quick_union  {
public:
    explicit weight_quick_union(const int count) : count(count), size(count), id(new int[count]), sz(new int[count]) {
        for (int i = 0; i < count; i++) {
            id[i] = i;
            sz[i] = 1;
        }
    }

    void union_operator(int p, int q)  {
        int qId = find(q);
        int pId = find(p);
        if (qId == pId)
            return;
        if (sz[pId] < sz[qId]) {
            id[pId] = qId;
            sz[qId] += sz[pId];
        } else {
            id[qId] = pId;
            sz[pId] += sz[qId];
        }
        size--;
    }

    bool connected(int p, int q)  {
        return find(p) == find(q);
    }

    ~weight_quick_union() {
        delete[] id;
        delete[] sz;
    }

private:
    int find(int p) {
        assert(p >= 0 && p < count);
        while (p != id[p])
            p = id[p];
        return p;
    }

private:
    int count;
    int size;
    int *id;
    int *sz;
};

class rank_quick_union  {
public:
    explicit rank_quick_union(const int count) : count(count), size(count), id(new int[count]), sz(new int[count]) {
        for (int i = 0; i < count; i++) {
            id[i] = i;
            sz[i] = 1;
        }
    }


    void union_operator(int p, int q)  {
        int qId = find(q);
        int pId = find(p);
        if (qId == pId)
            return;
        if (sz[pId] < sz[qId]) {
            id[pId] = qId;
        } else if (sz[pId] > sz[qId]) {
            id[qId] = pId;
        } else {
            id[pId] = qId;
            sz[pId]++;
        }
        size--;
    }

    bool connected(int p, int q)  {
        return find(p) == find(q);
    }

    ~rank_quick_union() {
        delete[] id;
        delete[] sz;
    }

private:
    int find(int p) {
        assert(p >= 0 && p < count);
        while (p != id[p])
            p = id[p];
        return p;
    }

private:
    int count;
    int size;
    int *id;
    int *sz;
};

class path_compress_quick_union  {
public:
    explicit path_compress_quick_union(const int count) : count(count), size(count), id(new int[count]),
                                                          sz(new int[count]) {
        for (int i = 0; i < count; i++) {
            id[i] = i;
            sz[i] = 1;
        }
    }


    void union_operator(int p, int q)  {
        int qId = find(q);
        int pId = find(p);
        if (qId == pId)
            return;
        if (sz[pId] < sz[qId]) {
            id[pId] = qId;
        } else if (sz[pId] > sz[qId]) {
            id[qId] = pId;
        } else {
            id[pId] = qId;
            sz[pId]++;
        }
        size--;
    }

    bool connected(int p, int q)  {
        return find(p) == find(q);
    }

    ~path_compress_quick_union() {
        delete[] id;
        delete[] sz;
    }

private:
    int find(int p) {
        assert(p >= 0 && p < count);
        while (p != id[p]) {
            id[p] = id[id[p]]; // 只增加了这一行代码
            p = id[p];
        }
        return p;
    }

private:
    int count;
    int size;
    int *id;
    int *sz;
};

template <typename union_find>
clock_t test_union_find(union_find&& uf, const int size) {
    std::vector<int> vl;
    std::vector<int> vr;
    for (int i = 0; i < size; i++) {
        vl.push_back(random() % size);
        vr.push_back(random() % size);
    }

    clock_t start = clock();

    for (int i = 0; i < size; i++) {
        uf.union_operator(vl[i], vr[i]);
    }

    for (int i = 0; i < size; i++) {
        assert(uf.connected(vl[i], vr[i]));
        assert(uf.connected(vr[i], vl[i]));
    }
    clock_t end = clock();
    return end - start;
}

int main() {

    int size = 10000000;

    std::cout << "weight_quick_union " << test_union_find(weight_quick_union(size), size) << std::endl;

   // cout << "rank_quick_union " << test_union_find(rank_quick_union(size), size) << endl;

    std::cout << "path_compress_quick_union " << test_union_find(path_compress_quick_union(size), size) << std::endl;

    return 0;
}