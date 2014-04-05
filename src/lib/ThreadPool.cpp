//File: ThreadPool.cpp
//Date: Fri Apr 04 20:49:40 2014 +0000
//Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#include "ThreadPool.hh"
#include "debugutils.h"

namespace __ThreadPoolImpl
{
	void worker(ThreadPool *tp) {
		for (; ;) {
			std::unique_lock<std::mutex> lock(tp->queue_mutex);
			if (tp->stop && tp->nr_active_thread == 0 && tp->tasks.empty())
				return;
			while (tp->tasks.empty()) {
		//		fprintf(stderr, "waiting %lu\n", tp->tasks.size());
				tp->condition.wait(lock);
		//		fprintf(stderr, "after waiting\n");
				if (tp->stop && tp->nr_active_thread == 0 && tp->tasks.empty())
					return;
			}

			m_assert(tp->tasks.size());

			std::function<void()> task(tp->tasks.top().second);
			tp->tasks.pop();

			lock.unlock();

			/*		// old one
			 *{
			 *    std::lock_guard<std::mutex> lock(tp->nr_active_thread_mutex);
			 *    tp->nr_active_thread ++;
			 *    //	fprintf(stderr, "++: %d, q:%d\n", tp->nr_active_thread, (int)tp->tasks.size());
			 *}
			 */
			__sync_fetch_and_add(&(tp->nr_active_thread), 1);
			task();
			{
			//	std::lock_guard<std::mutex> lock_queue(tp->queue_mutex);
				std::lock_guard<std::mutex> lock(tp->nr_active_thread_mutex);
				tp->nr_active_thread --;
			//	fprintf(stderr, "--: %d, q:%d\n", tp->nr_active_thread, (int)tp->tasks.size());
				if (tp->stop && tp->nr_active_thread == 0 && tp->tasks.empty()) {
			//		fprintf(stderr, "notify\n");
					tp->condition.notify_all();
					return;
				}
			}
		}
	}
}
