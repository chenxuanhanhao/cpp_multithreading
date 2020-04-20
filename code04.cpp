//死锁问题的产生和避免死锁的一般方法
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>

const int NUM = 100;
class LogFile {
public:
	LogFile() {
		f.open("log.txt");
	}


    // // 会产生死锁
	// void shared_print(std::string id, int value) {
	// 	// std::lock(m_mutex, m_mutex2);
	// 	std::lock_guard<std::mutex> locker(m_mutex);//,std::adopt_lock);
	// 	std::lock_guard<std::mutex> locker2(m_mutex2);//, std::adopt_lock);
	// 	std::cout<< "From" << id << ": " << value << std::endl;
	// }

	// void shared_print2(std::string id, int value) {
	// 	// std::lock(m_mutex, m_mutex2);
	// 	std::lock_guard<std::mutex> locker2(m_mutex2);//, std::adopt_lock);
	// 	std::lock_guard<std::mutex> locker(m_mutex);//, std::adopt_lock);
	// 	std::cout << "From" << id << ": " << value << std::endl;
	// }
    
    //std::lock(m_mutex, m_mutex2); + std::adopt_lock —— 结果正确
    void shared_print(std::string id, int value) {
		std::lock(m_mutex, m_mutex2);
		std::lock_guard<std::mutex> locker(m_mutex,std::adopt_lock);
		std::lock_guard<std::mutex> locker2(m_mutex2, std::adopt_lock);
		std::cout<< "From" << id << ": " << value << std::endl;
	}

	void shared_print2(std::string id, int value) {
		std::lock(m_mutex, m_mutex2);
		std::lock_guard<std::mutex> locker2(m_mutex2, std::adopt_lock);
		std::lock_guard<std::mutex> locker(m_mutex, std::adopt_lock);
		std::cout << "From" << id << ": " << value << std::endl;
	}

private:
	std::mutex m_mutex;
	std::mutex m_mutex2;
	std::ofstream f;
};

void function_1(LogFile& log) {
	for (int i = 0; i > -NUM; i--)
		log.shared_print("From t1: ", i);
}

int main() {
	LogFile log;
	std::thread t1(function_1, std::ref(log));
	for (int i = 0; i < NUM; i++)
		log.shared_print2("From main: ", i);
	t1.join();
	return 0;
}

