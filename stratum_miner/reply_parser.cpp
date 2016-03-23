#include "reply_parser.h"
#include "hex_string.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace stratum
{
	using namespace std;

	reply_parser::reply_parser(const string& json) :
		status_(false), 
		type_(Invalid)
	{
		boost::property_tree::ptree pt;

		std::istringstream is(json);
		boost::property_tree::read_json(is, pt);

		boost::optional<boost::property_tree::ptree&> params =
			pt.get_child_optional("params");
		if (!params)
		{
			std::string status = pt.get<string>("result.status");
			if (status == "OK")
				status_ = true;
			// get rpc id
			boost::optional<std::string> rpc_id =
				pt.get_optional<std::string>("result.id");
			if (rpc_id)
			{
				rpc_id_ = *rpc_id;
				type_ = LoginReply;
				params = pt.get_child_optional("result.job");
				if (!params)
					throw std::runtime_error("Invalid job json");
			}
			else
			{
				type_ = SubmitReply;
			}
		}
		else
			type_ = NewJob;

		if ((type_ == LoginReply) || (type_ == NewJob))
		{
			job_id_ = params->get<std::string>("job_id");
			blob_ = hex_string(params->get<std::string>("blob"));
			target_ = hex_string(params->get<std::string>("target"));
		}
	}

	const binary& reply_parser::blob() const
	{
		if ((type_ != LoginReply) && (type_ != NewJob))
			throw std::runtime_error("Invalid reply type");
		return blob_;
	}

	const std::string& reply_parser::job_id() const
	{
		if ((type_ != LoginReply) && (type_ != NewJob))
			throw std::runtime_error("Invalid reply type");
		return job_id_;
	}

	uint32_t reply_parser::target() const
	{
		if ((type_ != LoginReply) && (type_ != NewJob))
			throw std::runtime_error("Invalid reply type");
		return target_;
	}

	const std::string& reply_parser::rpc_id() const
	{
		if (type_ != LoginReply)
			throw std::runtime_error("Invalid reply type");
		return rpc_id_;
	}

	bool reply_parser::status() const
	{
		if ((type_ != LoginReply) && (type_ != SubmitReply))
			throw std::runtime_error("Invalid reply type");
		return status_;
	}

	reply_parser::Type reply_parser::type() const
	{
		return type_;
	}

}
