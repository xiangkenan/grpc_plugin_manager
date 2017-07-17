#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "ofo_main.grpc.pb.h"
#include "string_util.h"

using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::ClientAsyncResponseReader;

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
			CompletionQueue cq;
			ClientContext context;
			Status status;

			std::unique_ptr<ClientAsyncResponseReader<Response> >rpc(stub_kenan->AsyncChooseClassMethod(&context, request, &cq));
			rpc->Finish(response, &status, (void*)1);
			void *got_tag;
			bool ok=false;
			cq.Next(&got_tag, &ok);
			if(got_tag == (void*)1 && ok==true && status.ok())
			{
				return response->result();
			}
			else
			{
				return "{\"code\":-1,\"msg\":\"no param\"}";
			}
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
