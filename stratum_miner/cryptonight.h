#pragma once

#include "mining_algorithm.h"
#include <array>
#include <memory>


namespace stratum
{
	class cryptonight : public mining_algorithm
	{
	public:
		cryptonight(const binary& blob, uint32_t target,
			uint32_t start_nonce);

		virtual bool process_next_nonce() override;
		virtual uint32_t nonce() const override;
	private:
		std::array<uint32_t, 8> hash_;
	};
}

