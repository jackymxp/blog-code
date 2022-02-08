#include <list>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <iostream>

namespace cache {
    template<typename K, typename V>
    class LRUCache {
    public:
        typedef typename std::pair<K, V> value_type;
        typedef typename std::list<value_type>::iterator iterator;

        typedef K key_type;
        typedef const key_type &const_key_reference;

        typedef V val_type;
        typedef val_type &val_reference;
        typedef const val_type &const_val_reference;


        iterator begin() noexcept {
            return node_list.begin();
        }

        iterator end() noexcept {
            return node_list.end();
        }

        explicit LRUCache(size_t capacity = 16) : capacity(capacity) {}

        val_reference operator[](const_key_reference key) {
            if (!exists(key))
                put(key, {});
            return *get(key);
        }

        key_type operator[](const_key_reference key) const {
            return *get(key);
        }

        void put(const_key_reference key, const_val_reference value) {
            std::lock_guard<std::mutex> guard(mtx);
            auto it = node_map.find(key);
            if (it != node_map.end()) {
                node_list.erase(it->second);
                node_map.erase(it);
            }
            node_list.push_front(std::move(value_type(key, value)));
            node_map[key] = node_list.begin();

            if (node_map.size() > capacity) {
                auto last = node_list.end();
                last--;
                node_map.erase(last->first);
                node_list.pop_back();
            }
        }

        V *get(const K &key) {
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
                ks.push_back(node.first);
            return ks;
        }

        size_t size() const {
            return node_map.size();
        }

    private:
        std::list<value_type> node_list;
        std::unordered_map<K, iterator> node_map;
        size_t capacity;
        std::mutex mtx;
    };

} // namespace cache


#include <string>
#include <iostream>

int main(void) {
    using string = std::string;

    cache::LRUCache<string, int> lru(3);

    lru["a"] = 1;
    lru["b"] = 2;
    lru["c"] = 3;

    auto d = lru["d"];

    for (auto p : lru) {
        std::cout << p.first << " " << p.second << std::endl;
    }
}