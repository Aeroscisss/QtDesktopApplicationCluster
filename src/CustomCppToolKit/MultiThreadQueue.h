#include <mutex>
#include <condition_variable>
#include <queue>
/// <summary>
/// 消息队列
/// </summary>
/// <typeparam name="T">消息类型</typeparam>
template<class T> class MultiThreadQueue {
public:
	void push(const T& object) {//推入对象
		std::unique_lock<std::mutex>lck(_mtx);
		_queue.push(object);
		_cv.notify_one();
	}
	bool poll(T& object) {/// 轮询消息
		std::unique_lock<std::mutex>lck(_mtx);
		if (_queue.size()){
			object = _queue.front();
			_queue.pop();
			return true;
		}
		return false;
	}
	void wait(T& object) {/// 等待消息
		std::unique_lock<std::mutex>lck(_mtx);
		while (!_queue.size()) 
			_cv.wait(lck);
		object = _queue.front();
		_queue.pop();
	}
	size_t size() {//队列长度
		std::unique_lock<std::mutex>lck(_mtx);
		return _queue.size();
	}
private:
	std::queue<T> _queue;//队列
	std::mutex _mtx;//互斥变量
	std::condition_variable _cv;//条件变量
};