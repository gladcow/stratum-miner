#pragma once

#include <string>
#include <boost/asio.hpp>
#include "miner_pool.h"

namespace stratum
{
	class stratum_client
	{
	public:
		stratum_client(const std::string& server, const std::string& port,
			const std::string& login, const std::string& pwd);
		~stratum_client();
	private:
		void handle_resolve(const boost::system::error_code& err,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
		void handle_connect(const boost::system::error_code& err);
		void handle_login(const boost::system::error_code& err);
		void handle_new_job(const boost::system::error_code& err);
		void handle_job_succeeded(const std::string& job_id, int nounce);

		boost::asio::io_service io_service_;
		boost::asio::io_service::work working_;
		boost::asio::ip::tcp::resolver resolver_;
		boost::asio::ip::tcp::socket socket_;
		boost::asio::streambuf request_;
		boost::asio::streambuf response_;
		std::string login_;
		std::string pwd_;
		miner_pool pool_;
	};

}