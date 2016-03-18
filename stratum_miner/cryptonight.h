#pragma once

#include "mining_algorithm.h"

namespace stratum
{
	class cryptonight : public mining_algorithm
	{
	public:
		cryptonight(const binary& blob, double difficulty);

		virtual bool process_next_nounce() override;
		virtual unsigned nounce() const override;
	};
}

