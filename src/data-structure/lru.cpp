#include <list>
#include <unordered_map>
#include <vector>
#include <mutex>

namespace cache {
    template<typename K, typename V>
    class LRUCache {
    public:
        typedef typename std::pair<K, V> Node;
        typedef typename std::list<Node>::iterator list_iterator;

        explicit LRUCache(size_t capacity) : capacity(capacity) {}


        void put(const K &key, const V &value) {
            std::lock_guard<std::mutex> guard(mtx);
            auto it = node_map.find(key);
            if (it != node_map.end()) {
                node_list.erase(it->second);
                node_map.erase(it);
            }
            node_list.push_front(std::move(Node(key, value)));
            node_map[key] = node_list.begin();

            if (node_map.size() > capacity) {
                auto last = node_list.end();
                last--;
                node_map.erase(last->first);
                node_list.pop_back();
            }
        }

        const V *get(const K &key) {
            std::lock_guard<std::mutex> guard(mtx);
            auto it = node_map.find(key);
            if (it == node_map.end()) {
                //throw std::range_error("There is no such key in cache");
                return nullptr;
            } else {
                node_list.splice(node_list.begin(), node_list, it->second);
                return &(it->second->second);
            }
        }

        bool exists(const K &key) const {
            return node_map.find(key) != node_map.end();
        }

        std::vector<K> keys() const {
            std::vector<K> ks;
            for (const auto &node : node_list)
                ks.emplace_back(node.first);
            return ks;
        }

        size_t size() const {
            return node_map.size();
        }

    private:
        std::list<Node> node_list;
        std::unordered_map<K, list_iterator> node_map;
        size_t capacity;
        std::mutex mtx;
    };

}


#include <string>
#include <iostream>

int main(void) {
    using string = std::string;

    cache::LRUCache<string, int> lru(2);

    lru.put("a", 1);
    lru.put("b", 1);
    std::cout << "fdsaf" << std::endl;
    auto a = lru.get("a");
    std::cout << *a << std::endl;
    auto b = lru.get("b");
    std::cout << *b << " " << b << std::endl;
    auto d = lru.get("d");
    std::cout << d << std::endl;

    auto ks = lru.keys();
    std::cout << "keys " << std::endl;
    for (const auto &k : ks)
        std::cout << k << std::endl;

    a = lru.get("a");
    std::cout << *a << std::endl;
}