#ifndef TP3_TALLER2_BLOCKING_QUEUE_H
#define TP3_TALLER2_BLOCKING_QUEUE_H

#include <queue>
#include <mutex>
#include <utility>
#include <condition_variable>

// TODO usar movimiento

template <class T>
class BlockingQueue {
private:
	std::queue<T> queue;
	std::mutex mutex;
    bool closed_queue;
	std::condition_variable cv;
	
public:
	BlockingQueue<T>();

    BlockingQueue(const BlockingQueue<T> & orig);

    BlockingQueue(BlockingQueue<T> && orig);
	
	void push(const T & new_element);
	
	T pop();

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
BlockingQueue<T>::BlockingQueue(BlockingQueue<T> &&orig):
                                queue(std::move(orig.queue)), mutex(),
                                closed_queue(orig.closed_queue), cv() {}

template <class T>
void BlockingQueue<T>::push(const T & new_element) {
    std::unique_lock<std::mutex> u_lock(mutex);
    queue.push(new_element);
	cv.notify_all();
}

template <class T>
T BlockingQueue<T>::pop() {
	std::unique_lock<std::mutex> u_lock(mutex);
	cv.wait(u_lock, [&] {
		return !queue.empty() || closed_queue;
	});
    if (closed_queue) {
        throw std::runtime_error("se cerro el server mientras"
                                 " un cliente estaba esperando un mensaje");
    }
	T ret(queue.front());
	queue.pop();
	return ret;
}

template <class T>
void BlockingQueue<T>::unlock(){
    std::unique_lock<std::mutex> u_lock(mutex);
    closed_queue = true;
    cv.notify_all();
}


#endif //TP3_TALLER2_BLOCKING_QUEUE_H
