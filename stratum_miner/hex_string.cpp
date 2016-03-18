#include "hex_string.h"
#include <sstream>
#include <iomanip>

namespace stratum
{ 

	using namespace std;

	hex_string::hex_string(const string& hex_encoded) :
		encoded_(hex_encoded)
	{
		if ((hex_encoded.length() % 2) != 0)
			throw runtime_error("String has not valid length ...");

		size_t cnt = hex_encoded.length() / 2;

		for (size_t i = 0; cnt > i; ++i)
		{
			unsigned char s = strtol(hex_encoded.substr(i * 2, 2).c_str(), nullptr, 16);
			decoded_.push_back(s);
		}
	}

	hex_string::hex_string(const binary& bin_data) :
		decoded_(bin_data)
	{
		stringstream ss;
		for (auto b : bin_data)
			ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(b);
		encoded_ = ss.str();
	}

	hex_string::operator std::string && ()
	{
		return std::move(encoded_);
	}

	hex_string::operator binary && ()
	{
		return std::move(decoded_);
	}

	hex_string::operator uint32_t()
	{
		uint32_t res = 0;
		if (decoded_.size() > sizeof(uint32_t))
			throw "Failed convert hex string to uint32_t";
		memcpy(&res, &decoded_[0], decoded_.size());
		return res;
	}

}
