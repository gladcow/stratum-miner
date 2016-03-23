#pragma once
#include "common.h"
#include <memory>

namespace stratum
{

	class mining_algorithm
	{
	public:
		enum Algorithm
		{
			CRYPTONIGHT
		};

		static std::unique_ptr<mining_algorithm> factory(Algorithm a,
			const binary& blob, uint32_t target, uint32_t start_nonce);

		mining_algorithm(const binary& blob, uint32_t target,
			uint32_t start_nonce);

		virtual bool process_next_nonce() = 0;
		virtual uint32_t nonce() const = 0;
		virtual binary binary_res() const = 0;
	protected:
		binary blob_;
		uint32_t target_;
		uint32_t nonce_;
	};

}