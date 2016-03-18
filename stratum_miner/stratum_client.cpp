#include "stratum_client.h"
#include "job_parser.h"
#include <iostream>
#include <future>
#include <boost/bind.hpp>

namespace stratum
{
	stratum_client::stratum_client(const std::string& server, const std::string& port,
		const std::string& login, const std::string& pwd) :
		working_(io_service_),
		resolver_(io_service_),
		socket_(io_service_),
		login_(login), pwd_(pwd)
	{
		boost::asio::ip::tcp::resolver::query query(server, port);
		resolver_.async_resolve(query,
			boost::bind(&stratum_client::handle_resolve, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::iterator));
		std::async(boost::bind(&boost::asio::io_service::run, &io_service_));
	}

	stratum_client::~stratum_client()
	{
		io_service_.stop();
	}

	void stratum_client::handle_resolve(const boost::system::error_code& err,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	{
		if (!err)
		{
			boost::asio::async_connect(socket_, endpoint_iterator,
				boost::bind(&stratum_client::handle_connect, this,
				boost::asio::placeholders::error));
		}
		else
		{
			std::cout << "Error: " << err.message() << "\n";
		}
	}

	void stratum_client::handle_connect(const boost::system::error_code& err)
	{
		if (!err)
		{
			{
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
				boost::bind(&stratum_client::handle_login, this,
				boost::asio::placeholders::error));
		}
		else
		{
			std::cout << "Error: " << err.message() << "\n";
		}
	}


	void stratum_client::handle_login(const boost::system::error_code& err)
	{
		if (!err)
		{
			boost::asio::async_read_until(socket_, response_, "\n",
				boost::bind(&stratum_client::handle_new_job, this,
				boost::asio::placeholders::error));
		}
		else
		{
			std::cout << "Error: " << err << "\n";
		}
	}

	void stratum_client::handle_new_job(const boost::system::error_code& err)
	{
		if (!err)
		{
			std::ostringstream sout;
			sout << &response_;
			job_parser job(sout.str());
			pool_.set_job(job.blob(), job.difficulty(),
				boost::bind(&stratum_client::handle_job_succeeded, this, job.id(), _1));

			// Start reading remaining data until EOF.
			boost::asio::async_read_until(socket_, response_, "\n",
				boost::bind(&stratum_client::handle_new_job, this,
				boost::asio::placeholders::error));
		}
		else
		{
			std::cout << "Error: " << err << "\n";
		}
	}

	void stratum_client::handle_job_succeeded(const std::string& job_id, 
		int nounce)
	{
		std::cout << "job [" << job_id << "] succeeded, " << 
			nounce << std::endl;
	}

}