/*
 * Pool.cpp
 *
 *  Created on: Jul 10, 2019
 *      Author: Dylan Neff
 */


#include "ThreadPool.h"

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>

using namespace std;


ThreadPool2::ThreadPool() {
	pool_threads = thread::hardware_concurrency();
	unique_lock<mutex> lock(this->queue_mutex);
}

ThreadPool2::ThreadPool(int threads) {
	pool_threads = threads;
	unique_lock<mutex> lock(this->queue_mutex);
}

ThreadPool2::~ThreadPool2() {
	for(thread &th:threads) {
		th.join();
	}
}

void ThreadPool2::start_pool() {
	for(int i = 0; i < pool_threads; ++i) {

		threads.push_back(thread(&ThreadPool2::run_jobs, this));
	}
}

template<typename F, typename... Args>
void ThreadPool2::add_to_queue(F&& f, Args&&... args) {
	{
		unique_lock<mutex> lock(queue_mutex);
		jobs.push(bind(forward<F>(f), forward<Args>(args)...));
	}
}


void ThreadPool2::set_hardware_threads() {
	pool_threads = thread::hardware_concurrency();
}

void ThreadPool2::set_num_threads(int num) {
	pool_threads = num;
}

int ThreadPool2::get_threads() {
	return(pool_threads);
}

void ThreadPool2::run_jobs() {
	function<void()> job;

	while(true) {
		{
			unique_lock<mutex> lock(queue_mutex);
			if(!jobs.empty()) {
				job = jobs.front();
				jobs.pop();
			} else {
				return;
			}
		}
		job();
	}
}
