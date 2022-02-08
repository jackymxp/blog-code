#include <iostream>
#include <map>
#include <string>
#include <cassert>

using namespace std;

class Trie {
private:
    struct Node {
        bool isWord;
        map<char, Node *> next;

        Node() : isWord(false) {}
    };

    Node *root;
    int count;
public:
    Trie() : root(new Node()), count(0) {
        nodeCount++;
    }

    ~Trie() {
        destroy(root);
        assert(check());
    }


    int size() const { return count; }

    void add(const string &word) {
        Node *cur = root;
        for (const auto &c : word) {
            if ((cur->next).find(c) == cur->next.end()) {
                (cur->next)[c] = new Node();
                nodeCount++;
            }
            cur = (cur->next)[c];
        }
        if (!cur->isWord) {
            cur->isWord = true;
            count++;
        }
    }

    bool contain(const string &word) const {
        Node *cur = root;
        for (const auto &c : word) {
            if ((cur->next).find(c) == cur->next.end())
                return false;
            else
                cur = (cur->next)[c];
        }
        return cur->isWord;
    }

    bool isPrefix(const string &word) const {
        Node *cur = root;
        for (const auto &c : word) {
            if ((cur->next).find(c) == cur->next.end())
                return false;
            cur = (cur->next)[c];
        }
        return true;
    }

    bool check() const {
        return nodeCount == 0;
    }

private:
    void destroy(Node *node) {
        if (!node)
            return;

        for (const auto &p : node->next) {
            destroy(p.second);
        }

        delete node;
        nodeCount--;
    }

private:
    int nodeCount = 0;
};


int main() {
    cout << "test trie " << endl;

    Trie t;
    t.add("hello");
    t.add("world");
    t.add("hello");
    t.add("he");
    cout << t.size() << endl;

    cout << boolalpha << t.contain("he") << endl;
    cout << t.contain("hello") << endl;
    cout << t.contain("world") << endl;
    cout << t.contain("hahah") << endl;
    return 0;
}