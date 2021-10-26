#ifndef TP3_TALLER2_BLOCKING_QUEUE_H
#define TP3_TALLER2_BLOCKING_QUEUE_H

#include <queue>
#include <mutex>
#include <utility>
#include <condition_variable>

/* Clase template monitor de una cola. Es bloqueante,
 * por lo que a la hora de hacer el pop, si la cola se
 * encuentra vacia, el hilo que trate de leer quedara bloqueado
 * hasta que alguien agregue algo a la cola con push. */
template <class T>
class BlockingQueue {
private:
	std::queue<T> queue;
	std::mutex mutex;
    bool closed_queue;
	std::condition_variable cv;
	
public:
	BlockingQueue<T>();

    // Constructor por copia.
    BlockingQueue(const BlockingQueue<T> & orig);

    // Constructor por movimiento.
    BlockingQueue(BlockingQueue<T> && orig) noexcept;

    // Permite agregar un elemento al final de la cola.
	void push(const T & new_element);

    /* Obtiene el elemento del principio de la cola. Si la cola
     * esta vacia bloquea. */
	T pop();

    // Fuerza a desbloquear la cola.
    void unlock();
};

template <class T>
BlockingQueue<T>::BlockingQueue(): queue(), mutex(),
                        closed_queue(false), cv() {}

template<class T>
BlockingQueue<T>::BlockingQueue(const BlockingQueue<T> &orig):
                                queue(orig.queue), mutex(),
                                closed_queue(orig.closed_queue), cv() {}


template<class T>
BlockingQueue<T>::BlockingQueue(BlockingQueue<T> &&orig) noexcept:
                                queue(std::move(orig.queue)), mutex(),
                                closed_queue(orig.closed_queue), cv() {}

template <class T>
void BlockingQueue<T>::push(const T & new_element) {
    std::unique_lock<std::mutex> u_lock(mutex);
    // Se agrega el nuevo elemento.
    queue.push(new_element);
    // Se notifica a todos los threads que esperan.
	cv.notify_all();
}

template <class T>
T BlockingQueue<T>::pop() {
	std::unique_lock<std::mutex> u_lock(mutex);
    // Se espera hasta que haya algo en la cola o se cierre.
	cv.wait(u_lock, [&] {
		return !queue.empty() || closed_queue;
	});
    // Si se cerro se lanza una excepcion.
    if (closed_queue) {
        throw std::runtime_error("se cerro el server mientras"
                                 " un cliente estaba esperando un mensaje");
    }
    // Se obtiene el primer elemento y se popea.
	T ret(queue.front());
	queue.pop();
	return ret;
}

template <class T>
void BlockingQueue<T>::unlock(){
    std::unique_lock<std::mutex> u_lock(mutex);
    // Se marca la cola como cerrada.
    closed_queue = true;
    // Se notifica a todos los hilos que se cerro la cola.
    cv.notify_all();
}


#endif //TP3_TALLER2_BLOCKING_QUEUE_H
