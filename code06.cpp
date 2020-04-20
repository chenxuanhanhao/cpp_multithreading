//用条件变量等待条件

#include <deque>
#include <functional>
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>

std::deque<int> q;
std::mutex mu;
std::condition_variable cond;

//数据的生产者
void producer_func() {
	int count = 10;
	while (count > 0) {
		std::unique_lock<std::mutex> locker(mu);
		q.push_front(count);
		locker.unlock();
		//cond.notify_one();
		cond.notify_all();
		std::this_thread::sleep_for(std::chrono::seconds(1000));
		count--;
	}
}
//数据的消费者
void consumer_func() {
	int data = 0;
	while (data!=1) {
		std::unique_lock<std::mutex> locker(mu);
		//函数2可能会被伪激活，因此，传入lambda表达式作为第二个参数进行控制
		cond.wait(locker, []() {return !q.empty(); });
			data = q.back();
			q.pop_back();
			locker.unlock();
			std::cout << "consumer got a value from producer: " << data << std::endl;
	}
}

int main() {
	std::thread t1(producer_func);
	std::thread t2(consumer_func);
	t1.join();
	t2.join();

}
