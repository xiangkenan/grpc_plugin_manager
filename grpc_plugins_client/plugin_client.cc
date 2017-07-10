#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "ofo_main.grpc.pb.h"
#include "string_util.h"

using grpc::Status;
using grpc::Channel;

using std::string;
using std::vector;
using std::unordered_map;

class plugin_client
{
	public:
		plugin_client(std::shared_ptr<Channel> channel) : stub_kenan(plugins_rpc::NewStub(channel)) {}

		std::string ChooseClassMethod(const string& params, Response *response)
		{
			Request request;

			parse_params(params, &request);

			grpc::ClientContext context;
			stub_kenan->ChooseClassMethod(&context, request, response);
			return response->result();
		}	
	private:
		bool parse_params(const string& params, Request* request);

		std::unique_ptr<plugins_rpc::Stub> stub_kenan;
};

bool plugin_client::parse_params(const string& params, Request* request)
{
	vector<string> params_field;
	Split(params, "&", &params_field);
	for (unsigned int i=0; i<params_field.size(); i++)
	{
		vector <string> linshi;
		Split(params_field[i], "=", &linshi);
		if(linshi.size() == 2)
		{
			auto map = request->mutable_params();
			(*map)[linshi[0]] = linshi[1];
		}
	}

	return true;
}
