#pragma once

#include "mining_algorithm.h"
#include <array>
#include <memory>

struct cryptonight_ctx;

namespace stratum
{
	class cryptonight : public mining_algorithm
	{
	public:
		cryptonight(const binary& blob, uint32_t target,
			uint32_t start_nonce);

		virtual bool process_next_nonce() override;
		virtual uint32_t nonce() const override;
		virtual binary binary_res() const override;
	private:
		std::array<uint32_t, 8> hash_;
		std::unique_ptr<struct cryptonight_ctx, 
			void(*)(struct cryptonight_ctx*)> ctx_;
		bool aes_ni_support_;
	};
}

