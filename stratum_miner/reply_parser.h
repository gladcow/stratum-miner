#pragma once

#include <string>
#include <stdint.h>
#include "common.h"

namespace stratum
{
	////////////////////////////////////////////////////////////
	// Class to parse json with job data from the stratum 
	// server. Can parse login reply and new job push jsons.
	// Example source data:
	// Successful login reply:
	// {"id":1,"jsonrpc":"2.0","error":null,"result":{"id":"554861464863643","job":{"blob":"010089f1d7b605e9888a1269f4c315f599975047a83d97c5eefe186c18e72dc1fd0848f22cba0000000000c5863cf2244ea3fa57eea58bb49211d0037e19e557004e5f7afe4df3dc88f11101","job_id":"645492095453664","target":"169f0200"},"status":"OK"}}
	// New job push:
	// {"jsonrpc":"2.0","method":"job","params":{"blob":"0100b9f1d7b6050e75a450bdb677fe14caba3d0383ff13dd8fd94175a0c84cf6eaeae3ff167e0900000000a67c724237d89fcac81b5ff6de608f7a8279c6d61dd3f1ca28abaa27a3774e3501","job_id":"311512568756006","target":"169f0200"}}
	// Also can parse submit reply
	///////////////////////////////////////////////////////////
	class reply_parser
	{
	public:
		reply_parser(const std::string& json);

		const binary& blob() const;
		const std::string& job_id() const;
		uint32_t target() const;

		enum Type
		{
			Invalid = 0,
			NewJob,
			LoginReply,
			SubmitReply
		};
		Type type() const;

		// only for login json
		const std::string& rpc_id() const;
		// only for login and submit reply
		bool status() const;
	private:
		binary blob_;
		std::string job_id_;
		uint32_t target_;
		std::string rpc_id_;
		bool status_;
		Type type_;
	};

}

