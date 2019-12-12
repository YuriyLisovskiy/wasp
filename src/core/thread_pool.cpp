/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * An implementation of thread_pool.h.
 */

#include "thread_pool.h"


__INTERNAL_BEGIN__

ThreadPool::ThreadPool(size_t threads_count) : _threads(threads_count)
{
	this->_threads_count = threads_count;
	for (auto& thread : this->_threads)
	{
		thread = std::thread(&ThreadPool::_thread_handler, this);
	}
}

ThreadPool::~ThreadPool()
{
	// Signal to dispatch threads that it's time to wrap up.
	std::unique_lock<std::mutex> lock(this->_lock);
	this->_quit = true;
	lock.unlock();
	this->_cond_var.notify_all();

	// Wait for threads to finish before we exit.
	for (auto& thread : this->_threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
}

void ThreadPool::push(const std::function<void(void)>& func)
{
	std::unique_lock<std::mutex> lock(this->_lock);
	this->_queue.push(func);

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	lock.unlock();
	this->_cond_var.notify_all();
}

void ThreadPool::push(std::function<void(void)>&& func)
{
	std::unique_lock<std::mutex> lock(this->_lock);
	this->_queue.push(std::move(func));

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	lock.unlock();
	this->_cond_var.notify_all();
}

size_t ThreadPool::threads_count()
{
	return this->_threads_count;
}

void ThreadPool::_thread_handler()
{
	std::unique_lock<std::mutex> lock(this->_lock);
	do
	{
		// Wait until we have data or a quit signal.
		this->_cond_var.wait(lock, [this] {
			return (!this->_queue.empty() || this->_quit);
		});

		// After wait, we own the lock.
		if (!this->_quit && !this->_queue.empty())
		{
			auto func = std::move(this->_queue.front());
			this->_queue.pop();

			// Unlock now that we're done messing with the queue.
			lock.unlock();

			func();

			lock.lock();
		}
	} while (!this->_quit);
}

__INTERNAL_END__
