//将mutex和受保护对象组织为一个类

//mutex 只负责保护数据不被多线程同时写入而破坏，不保证顺序性。
//release 版本交替打印也只是巧合。两个线程还指望顺序性本身就没有意义，如果要顺序性，为什么不一个线程解决？
//多线程的 happens before 顺序需要靠条件变量或者原子操作实现。

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>

const int NUM = 10;
class LogFile {
public:
	LogFile() {
		f.open("log.txt");
	}
	void shared_print(std::string id, int value){
		std::lock_guard<std::mutex> locker(m_mutex);
		f << "From"<< id << ":\t"<< value << std::endl;
	}
private:
	std::mutex m_mutex;
	std::ofstream f;
};

void function_1(LogFile& log) {
	for (int i = 0; i > -NUM; i--)
		log.shared_print("From t1:\t",i);
}

/*在debug版本中依然会出现部分顺序错乱的现象，
但在release版本则完全正常，本人猜测是因为release中运算速度最优*/
int main() {
	LogFile log;
	std::thread t1(function_1, std::ref(log));

	for (int i = 0; i < NUM; i++) 
		log.shared_print("From main:\t", i);
	t1.join();
	return 0;
}