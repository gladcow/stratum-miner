#include "cryptonight.h"
#include "../hashes/hashes.h"

namespace stratum
{


	cryptonight::cryptonight(const binary& blob, uint32_t target, 
		uint32_t start_nonce) :
		mining_algorithm(blob, target, start_nonce)
	{
		if (blob_.size() < 76)
			throw std::runtime_error("Invalid blob for cryptonight hash");
	}

	bool cryptonight::process_next_nonce()
	{
		nonce_++;
		// copy nonce to the blob data
		uint32_t *nonceptr = (uint32_t*)(&blob_[39]);
		*nonceptr = nonce_;
		// calc hash
		cryptonight_hash(hash_.data(), &blob_[0], 76);
		return (hash_[7] < target_);
	}

	uint32_t cryptonight::nonce() const
	{
		return nonce_;
	}

}