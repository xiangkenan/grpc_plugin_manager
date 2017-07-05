#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "ofo_main.grpc.pb.h"
#include "plugin_manager.h"

bool RunServer(int argc, char *argv[])
{
	std::string server_address("0.0.0.0:99999");

	grpc::ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

	//插件管理框架
	PluginManager service;
	if(service.Init(argc, argv) == false)
	{
		LOG(ERROR) << "manager init error";
		return false;
	}

	builder.RegisterService(&service);

	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;

	server->Wait();
}

int main(int argc, char *argv[])
{
	RunServer(argc, argv);
	return 0;
}
