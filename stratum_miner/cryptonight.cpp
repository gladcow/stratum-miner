#include "cryptonight.h"
#include "../hashes/hashes.h"

namespace stratum
{

	cryptonight::cryptonight(const binary& blob, uint32_t target, 
		uint32_t start_nonce) :
		mining_algorithm(blob, target, start_nonce),
		ctx_(cryptonight_alloc_ctx(), cryptonight_free_ctx),
		aes_ni_support_(has_aes_ni_support() ? true : false)
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
		if (aes_ni_support_)
			cryptonight_hash_ctx_aes_ni(hash_.data(), &blob_[0], 76, ctx_.get());
		else
			cryptonight_hash_ctx(hash_.data(), &blob_[0], 76, ctx_.get());
		return (hash_[7] < target_);
	}

	uint32_t cryptonight::nonce() const
	{
		return nonce_;
	}

	binary cryptonight::binary_res() const
	{
		const unsigned char* start = 
			reinterpret_cast<const unsigned char*>(&hash_[0]);
		return binary(start, start + hash_.size() * sizeof(uint32_t));
	}

}