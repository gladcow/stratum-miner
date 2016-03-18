#include "mining_algorithm.h"
#include "cryptonight.h"


namespace stratum{


	std::unique_ptr<mining_algorithm> 
		mining_algorithm::factory(Algorithm a, const binary& blob, 
		double difficulty)
	{
		switch (a)
		{
		case(CRYPTONIGHT) :
			return std::make_unique<cryptonight>(blob, difficulty);
		}
		throw std::runtime_error("Failed to create unknown mining algorithm object");
	}

	mining_algorithm::mining_algorithm(const binary& blob, double difficulty) :
		blob_(blob), 
		difficulty_(difficulty),
		nounce_(0)
	{

	}

}