#pragma once

#include <string>
#include <stdint.h>
#include "common.h"

namespace stratum
{
	class hex_string
	{
	public:
		hex_string(const std::string& hex_encoded);
		hex_string(const binary& bin_data);

		operator std::string && ();
		operator binary && ();
		operator uint32_t();
	private:
		binary decoded_;
		std::string encoded_;
	};
}

