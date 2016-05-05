#pragma once

#include <boost/function.hpp>
#include <boost/asio/io_service.hpp>
#include <thread>
#include <atomic>
#include "common.h"
#include "hash_counter.h"

namespace stratum
{
	class miner_pool
	{
	public:
		typedef boost::function<void(uint32_t, binary)> job_callback;

		miner_pool(unsigned thread_num = std::thread::hardware_concurrency());
		~miner_pool();

		void set_job(const binary& blob, uint32_t target, job_callback cb);
		void stop_current_job();

		double hash_per_second() const;
	private:
		static void calc(const binary& blob, uint32_t target, 
			uint32_t start_nonce, job_callback cb, std::atomic_flag& stop, 
			hash_counter& hashes);

		void stop();
		void start();

		boost::asio::io_service io_service_;
		boost::asio::io_service::work working_;
		std::vector<std::thread> pool_;
		unsigned thread_num_;
		std::atomic_flag stop_flag_;
		hash_counter hashes_;
	};
}
