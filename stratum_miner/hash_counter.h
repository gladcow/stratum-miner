#pragma once
#include <chrono>
#include <atomic>

namespace stratum
{
	class hash_counter
	{
	public:
		hash_counter();

		void add_hashes(unsigned hashes);
		double hash_per_second() const;
	private:
		const time_t start_;
		std::atomic<unsigned long long> hashes_;
	};

}
