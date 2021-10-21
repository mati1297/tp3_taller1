#ifndef TP3_TALLER2_BLOCKING_QUEUE_H
#define TP3_TALLER2_BLOCKING_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class BlockingQueue {
private:
	std::queue<T> queue;
	std::mutex mutex;
	std::condition_variable cv;
	
public:
	BlockingQueue<T>();
	
	void push(const T & new_element);
	
	T pop();
};

template <class T>
BlockingQueue<T>::BlockingQueue(): queue(), mutex(), cv() {}

template <class T>
void BlockingQueue<T>::push(const T & new_element) {
    std::unique_lock<std::mutex> u_lock(mutex);
    queue.push(new_element);
	cv.notify_one();
}

template <class T>
T BlockingQueue<T>::pop() {
	std::unique_lock<std::mutex> u_lock(mutex);
	cv.wait(lock, [=] {
		return !this->queue.empty();
	});
	T ret(queue.front());
	queue.pop();
	return ret;
}

#endif //TP3_TALLER2_BLOCKING_QUEUE_H
