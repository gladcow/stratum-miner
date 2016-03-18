#include "job_parser.h"
#include "hex_string.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace stratum
{
	using namespace std;

	job_parser::job_parser(const string& json)
	{
		boost::property_tree::ptree pt;

		std::istringstream is(json);
		boost::property_tree::read_json(is, pt);

		boost::optional<boost::property_tree::ptree&> params =
			pt.get_child_optional("params");
		if (!params)
			params = pt.get_child_optional("result.job");
		if (!params)
			throw std::runtime_error("Invalid job json");

		job_id_ = params->get<std::string>("job_id");
		blob_ = hex_string(params->get<std::string>("blob"));
		uint32_t target = hex_string(params->get<std::string>("target"));
		difficulty_ = (((double)0xffffffff) / target);
	}

	const binary& job_parser::blob() const
	{
		return blob_;
	}

	const std::string& job_parser::id() const
	{
		return job_id_;
	}

	double job_parser::difficulty() const
	{
		return difficulty_;
	}

}
