#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "ofo_main.grpc.pb.h"

using grpc::Status;
using grpc::Channel;

class test_client
{
	public:
		test_client(std::shared_ptr<Channel> channel) : stub_kenan(ofo_user_rpc::NewStub(channel)) {}

		std::string oo()
		{
			request tt;
			tt.set_class_name("nihaoa, kenan!");

			response qq;
			grpc::ClientContext context;
			stub_kenan->ChooseClassMethod(&context, tt, &qq);
			std::cout << qq.result() << std::endl;
			return qq.result();
		}	
	private:
		std::unique_ptr<ofo_user_rpc::Stub> stub_kenan;
};


int main(int argc, char *argv[])
{
	test_client aa(grpc::CreateChannel("localhost:99999", grpc::InsecureChannelCredentials()));
	aa.oo();
	return 0;
}
