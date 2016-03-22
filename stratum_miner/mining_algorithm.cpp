#include "mining_algorithm.h"
#include "cryptonight.h"


namespace stratum{


	std::unique_ptr<mining_algorithm> 
		mining_algorithm::factory(Algorithm a, const binary& blob, 
		uint32_t target, uint32_t start_nonce)
	{
		switch (a)
		{
		case(CRYPTONIGHT) :
			return std::make_unique<cryptonight>(blob, target, start_nonce);
		}
		throw std::runtime_error("Failed to create unknown mining algorithm object");
	}

	mining_algorithm::mining_algorithm(const binary& blob, uint32_t target,
		uint32_t start_nonce) :
		blob_(blob), 
		target_(target),
		nonce_(start_nonce)
	{
	}

}