#include "hash_counter.h"

namespace stratum
{

	hash_counter::hash_counter() :
		start_(time(nullptr)),
		hashes_(0)
	{
	}

	void hash_counter::add_hashes(unsigned hashes)
	{
		hashes_ += hashes;
	}

	double hash_counter::hash_per_second() const
	{
		unsigned long long val = hashes_.load();
		unsigned secs = time(nullptr) - start_;
		if (secs == 0)
			return 0.0;
		return double(val) / secs;
	}

}