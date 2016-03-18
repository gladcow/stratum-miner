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
			const binary& blob, double difficulty);

		mining_algorithm(const binary& blob, double difficulty);

		virtual bool process_next_nounce() = 0;
		virtual unsigned nounce() const = 0;

	protected:
		binary blob_;
		double difficulty_;
		unsigned nounce_;
	};

}