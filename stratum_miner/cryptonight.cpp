#include "cryptonight.h"

namespace stratum
{


	cryptonight::cryptonight(const binary& blob, double difficulty) :
		mining_algorithm(blob, difficulty)
	{

	}

	bool cryptonight::process_next_nounce()
	{
		return false;
	}

	unsigned cryptonight::nounce() const
	{
		return nounce_;
	}

}