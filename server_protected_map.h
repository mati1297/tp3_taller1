#ifndef TP3_TALLER1_SERVER_PROTECTED_MAP_H
#define TP3_TALLER1_SERVER_PROTECTED_MAP_H

#include <map>
#include <utility>

// TODO aclarar que es de blocking queues.

template <class Key, class Val>
class ProtectedMap {
private:
    std::map<Key, BlockingQueue<Val>> map;
    std::mutex mutex;

public:
    ProtectedMap();

    BlockingQueue<Val> & at(const Key & key);

    void insert(const std::pair<Key, BlockingQueue<Val>> & pair);

    void unlockAll();
};

template<class Key, class Val>
ProtectedMap<Key, Val>::ProtectedMap(): map(), mutex() {}

template <class Key, class Val>
BlockingQueue<Val> & ProtectedMap<Key, Val>::at(const Key & key) {
    std::lock_guard<std::mutex> lock_guard(mutex);
    return map.at(key);
}

template<class Key, class Val>
void ProtectedMap<Key, Val>::insert(const std::pair<Key,
                                    BlockingQueue<Val>> & pair) {
    std::lock_guard<std::mutex> lock_guard(mutex);
    map.insert(pair);
}

template<class Key, class Val>
void ProtectedMap<Key, Val>::unlockAll() {
    std::lock_guard<std::mutex> lock_guard(mutex);
    for (auto it = map.begin(); it != map.end(); ++it)
        it->second.unlock();
}

#endif //TP3_TALLER1_SERVER_PROTECTED_MAP_H
