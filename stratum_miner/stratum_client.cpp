#include "stratum_client.h"
#include "reply_parser.h"
#include "hex_string.h"
#include <iostream>
#include <future>
#include <boost/bind.hpp>
#include <thread>

namespace stratum
{

	void default_error_callback(int error, const std::string& msg,
		stratum_client* client)
	{
		std::cout << "Error: " << error << ": " << msg << "\n";
		client->async_reconnect();
	}

	stratum_client::stratum_client(const std::string& server, const std::string& port,
		const std::string& login, const std::string& pwd, error_callback ec) :
		pool_(7),
		working_(io_service_),
		resolver_(io_service_),
		socket_(io_service_),
		server_(server), port_(port),
		login_(login), pwd_(pwd),
		inited_(false), 
		ec_(ec)
	{
		std::async(boost::bind(&boost::asio::io_service::run, &io_service_));
		io_service_.post(boost::bind(&stratum_client::reconnect, this));
	}

	stratum_client::~stratum_client()
	{
		io_service_.stop();
	}

	void stratum_client::async_reconnect()
	{
		inited_ = false;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		io_service_.post(boost::bind(&stratum_client::reconnect, this));
	}

	void stratum_client::reconnect()
	{
		pool_.stop_current_job();
		std::cout << "\n****\nStart connect to " << server_ << 
			":" << port_ << "..." << std::endl;
		boost::asio::ip::tcp::resolver::query query(server_, port_);
		inited_ = true;
		resolver_.async_resolve(query,
			boost::bind(&stratum_client::handle_resolve, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::iterator));
	}

	void stratum_client::handle_resolve(const boost::system::error_code& err,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	{
		if (!inited_)
			return;
		if (!err)
		{
			boost::asio::async_connect(socket_, endpoint_iterator,
				boost::bind(&stratum_client::handle_connect, this,
				boost::asio::placeholders::error));
		}
		else
		{
			ec_(err.value(), err.message(), this);
		}
	}

	void stratum_client::handle_connect(const boost::system::error_code& err)
	{
		if (!inited_)
			return;
		if (!err)
		{
			{
				std::cout << "Try to login..." << std::endl;
				std::ostream request_stream(&request_);
				request_stream << "{\"id\": 1, \"method\": \"login\", "
					"\"params\": { "
					"\"login\":\"" << login_ << "\","
					"\"pass\" : \"" << pwd_ << "\", "
					"\"agent\" : \"embedded_miner/1.0\""
					"}"
					"}\n";
			}

			boost::asio::async_write(socket_, request_,
				boost::bind(&stratum_client::handle_write_completed, this,
				boost::asio::placeholders::error));
		}
		else
		{
			ec_(err.value(), err.message(), this);
		}
	}

	void stratum_client::handle_job_succeeded(const std::string& job_id, 
		uint32_t nonce, const binary& hash)
	{
		std::cout << "job [" << job_id << "] succeeded, " <<
			nonce << std::endl;
		{
			std::ostringstream oss;
			oss << "{\"method\": \"submit\", \"params\": "
				"{\"id\": \"" << rpc_id_ << "\", \"job_id\": \"" << job_id <<
				"\", \"nonce\": \"" << std::string(hex_string(nonce)) <<
				"\", \"result\": \"" << std::string(hex_string(hash)) <<
				"\"}, \"id\":1}\n";
			std::ostream request_stream(&request_);
			request_stream << oss.str();
		}

		boost::asio::async_write(socket_, request_,
			boost::bind(&stratum_client::handle_write_completed, this,
			boost::asio::placeholders::error));
	}

	void stratum_client::handle_write_completed(const boost::system::error_code& err)
	{
		if (!err)
		{
			boost::asio::async_read_until(socket_, response_, "\n",
				boost::bind(&stratum_client::handle_server_msg, this,
				boost::asio::placeholders::error));
		}
		else
		{
			ec_(err.value(), err.message(), this);
		}
	}

	void stratum_client::handle_server_msg(const boost::system::error_code& err)
	{
		if (!err)
		{
			std::ostringstream sout;
			sout << &response_;
			reply_parser job(sout.str());
			if (job.type() == reply_parser::LoginReply)
			{
				if (!job.status())
				{
					std::cout << "Login failed" << std::endl;
					return;
				}
				std::cout << "Login succeeded" << std::endl;
				rpc_id_ = job.rpc_id();
			}
			if ((job.type() == reply_parser::LoginReply) ||
				(job.type() == reply_parser::NewJob))
			{
				std::cout << "New job detected" << std::endl;
				pool_.set_job(job.blob(), job.target(),
					boost::bind(&stratum_client::handle_job_succeeded, 
					this, job.job_id(), _1, _2));
				std::cout << pool_.hash_per_second() <<
					" hashes per second" << std::endl;
			}
			if (job.type() == reply_parser::SubmitReply)
			{
				if (job.status())
					std::cout << "Submit succeeded" << std::endl;
				else
					std::cout << "Submit failed" << std::endl;
			}

			// Start reading data again.
			boost::asio::async_read_until(socket_, response_, "\n",
				boost::bind(&stratum_client::handle_server_msg, this,
				boost::asio::placeholders::error));
		}
		else
		{
			ec_(err.value(), err.message(), this);
		}
	}

}