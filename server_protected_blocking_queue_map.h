#ifndef TP3_TALLER1_SERVER_PROTECTED_BLOCKING_QUEUE_MAP_H
#define TP3_TALLER1_SERVER_PROTECTED_BLOCKING_QUEUE_MAP_H

#include <map>
#include <utility>

/* Clase template monitor de un map cuyo val es
 * BlockingQueue. Key es la clase clave y BQVal
 * es la clase que contiene la Blocking Queue. */
template <class Key, class BQVal>
class ProtectedBlockingQueueMap {
private:
    std::map<Key, BlockingQueue<BQVal>> map;
    std::mutex mutex;

public:
    // Constructor.
    ProtectedBlockingQueueMap();

    // Agrega un elemento al map.
    void insert(const std::pair<Key, BlockingQueue<BQVal>> & pair);

    // Debloquea todas las colas.
    void unlockAll();

    /* Devuelve una referencia a la blocking queue correspondiente a key.
     * (ver aclaracion en la definicion del metodo). */
    BlockingQueue<BQVal> & at(const Key & key);
};

template<class Key, class Val>
ProtectedBlockingQueueMap<Key, Val>::ProtectedBlockingQueueMap(): map(),
                                                                  mutex() {}

/* Este metodo esta protegido pero luego, la BlockingQueue podria (y lo va a hacer en
 * el programa) ser modificada. Sin embargo, segun la documentacion:
 * "Data races:
 * The container is accessed (neither the const nor the non-const
 * versions modify the container). The mapped value that is accessed may
 * be modified by the caller. Concurrently accessing or modifying other elements is safe."
 * Establece que el map en si no va a ser modificado y es seguro modificar o acceder
 * otros elementos, por lo que no existe data race. Podria sin embargo ocurrir en el
 * elemento devuelto, pero la clase BlockingQueue ya esta protegida por si misma. */
template <class Key, class Val>
BlockingQueue<Val> & ProtectedBlockingQueueMap<Key, Val>::at(const Key & key) {
    std::lock_guard<std::mutex> lock_guard(mutex);
    return map.at(key);
}

template<class Key, class Val>
void ProtectedBlockingQueueMap<Key, Val>::insert(const std::pair<Key,
                                    BlockingQueue<Val>> & pair) {
    std::lock_guard<std::mutex> lock_guard(mutex);
    map.insert(std::move(pair));
}

template<class Key, class Val>
void ProtectedBlockingQueueMap<Key, Val>::unlockAll() {
    std::lock_guard<std::mutex> lock_guard(mutex);
    // Itera los pares de key y val, y desbloquea las colas.
    for (auto it = map.begin(); it != map.end(); ++it)
        it->second.unlock();
}

#endif //TP3_TALLER1_SERVER_PROTECTED_BLOCKING_QUEUE_MAP_H
