#if !defined(__LOCKEDQUEUE)
#define __LOCKEDQUEUE
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>

//locks the thread waiting for something present in the queue
template<class T>
class locked_queue {
	public:
	locked_queue():cancelled(false) {}
	void push(T v){
		// wait for the pop finish
		//{
			std::unique_lock<std::mutex> lk(m);
			//cv.wait(lk, []{return processed;});
		//}
		queue.push(v);
		cv.notify_one();
	}
	T pop(){
		// Wait until something in queue
		//{
			std::unique_lock<std::mutex> lk(m2);
			cv.wait(lk, [&]{ return (!queue.empty() or cancelled);});
		//}
		//if(cancelled){
		//	return nullptr;
		//}
		T val = queue.front();
		queue.pop();
		return val;
	}
	void cancel(bool c){
		std::unique_lock<std::mutex> lk(m2);
		cancelled = c;
		cv.notify_one();
	}
	bool cancelled;
	private:
	std::mutex m;
	std::mutex m2;
	std::condition_variable cv;

	std::queue<T> queue;
};
#endif //__LOCKEDQUEUE