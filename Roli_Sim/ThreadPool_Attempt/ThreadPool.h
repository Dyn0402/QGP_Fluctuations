/*
 * Pool.h
 *
 *  Created on: Jul 10, 2019
 *      Author: Dylan Neff
 *
 *  Modeled on/copied from progschj ThreadPool (github)
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

using namespace std;


class ThreadPool2
{
public:

	ThreadPool();
	ThreadPool(int threads);
	~ThreadPool2();

	void start_pool();

	template<typename F, typename... Args>
	void add_to_queue(F&& f, Args&&... args);

	void set_num_threads(int num);
	void set_hardware_threads();

	int get_threads();

private:
	vector<thread> threads;
	queue<function<void()>> jobs;

	int pool_threads;

	mutex queue_mutex;
//	condition_variable condition;

	void run_jobs();
};


#endif /* THREADPOOL_H_ */
