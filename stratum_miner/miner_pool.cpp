#include "miner_pool.h"
#include "mining_algorithm.h"
#include <iostream>
#include <boost/bind.hpp>

namespace stratum
{
	miner_pool::miner_pool(unsigned thread_num) :
		working_(io_service_),
		thread_num_(thread_num)
	{
		start();
	}

	miner_pool::~miner_pool()
	{
		stop();
	}

	void miner_pool::set_job(const binary& blob, double difficulty, job_callback cb)
	{
		stop();
		for (unsigned i = 0; i < thread_num_; i++)
			io_service_.post(boost::bind(&miner_pool::calc, blob,
			difficulty, cb, std::ref(stop_flag_)));
		start();
	}

	void miner_pool::calc(const binary& blob, double difficulty, job_callback cb, 
		std::atomic_flag& stop)
	{
		std::cout << "Start calc..." << std::endl;
		std::unique_ptr<mining_algorithm> alg =
			mining_algorithm::factory(mining_algorithm::CRYPTONIGHT, blob,
			difficulty);
		while (stop.test_and_set())
		{
			if (alg->process_next_nounce())
				cb(alg->nounce());
		}
		stop.clear();
	}

	void miner_pool::stop()
	{
		io_service_.stop();
		stop_flag_.clear();
		for (auto& t : pool_)
			t.join();
		pool_.clear();
	}

	void miner_pool::start()
	{
		stop_flag_.test_and_set();
		io_service_.reset();
		for (unsigned i = 0; i < thread_num_; i++)
		{
			pool_.push_back(
				std::thread(
					boost::bind(&boost::asio::io_service::run, &io_service_)
				)
			);
		}
	}

}