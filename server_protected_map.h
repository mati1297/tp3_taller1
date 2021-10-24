#ifndef TP3_TALLER1_SERVER_PROTECTED_MAP_H
#define TP3_TALLER1_SERVER_PROTECTED_MAP_H

#include <map>
#include <utility>

template <class Key, class Val>
class ProtectedMap {
private:
    std::map<Key, Val> map;
    std::mutex mutex;

public:
    ProtectedMap();

    Val & at(const Key & key);

    void insert(const std::pair<Key, Val> & pair);
};

template<class Key, class Val>
ProtectedMap<Key, Val>::ProtectedMap(): map(), mutex() {}

template <class Key, class Val>
Val & ProtectedMap<Key, Val>::at(const Key & key) {
    std::lock_guard<std::mutex> lock_guard(mutex);
    return map.at(key);
}

template<class Key, class Val>
void ProtectedMap<Key, Val>::insert(const std::pair<Key, Val> & pair) {
    std::lock_guard<std::mutex> lock_guard(mutex);
    map.insert(pair);
}


#endif //TP3_TALLER1_SERVER_PROTECTED_MAP_H
