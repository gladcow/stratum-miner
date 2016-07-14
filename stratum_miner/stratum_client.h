#pragma once
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include "miner_pool.h"

namespace stratum
{
	class stratum_client;

	void default_error_callback(int error, const std::string& msg,
		stratum_client* client);

	class stratum_client : 
		public std::enable_shared_from_this<stratum_client>
	{
	public:
		typedef void(*error_callback)(int error, const std::string& msg, 
			stratum_client* client);

		stratum_client(const std::string& server, const std::string& port,
			const std::string& login, const std::string& pwd, 
			error_callback ec = default_error_callback);
		~stratum_client();

		void async_reconnect();
	private:
		void reconnect();
		void handle_resolve(const boost::system::error_code& err,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
		void handle_connect(const boost::system::error_code& err);
		void handle_server_msg(const boost::system::error_code& err);
		void handle_job_succeeded(const std::string& job_id, uint32_t nonce,
			const binary& hash);
		void handle_write_completed(const boost::system::error_code& err);

		boost::asio::io_service io_service_;
		boost::asio::io_service::work working_;
		boost::asio::ip::tcp::resolver resolver_;
		boost::asio::ip::tcp::socket socket_;
		boost::asio::streambuf request_;
		boost::asio::streambuf response_;
		std::string server_;
		std::string port_;
		std::string login_;
		std::string pwd_;
		std::string rpc_id_;
		miner_pool pool_;
		bool inited_;
		error_callback ec_;
	};

}