#include <iostream>
#include <grpc++/grpc++.h>
#include <getopt.h>
#include "glog/logging.h"

#include "ofo_main.grpc.pb.h"
#include "log.h"
#include "plugin_manager.h"

using namespace std;

class PluginManager final : public ofo_user_rpc::Service
{
	public:
		grpc::Status ChooseClassMethod(grpc::ServerContext *context, const request *cc, response *ss);
		PluginManager():conf_path_(NULL), log_path_(NULL) {}

		bool Init(int argc, char *argv[]);

	private:
		bool ParseParam(int argc, char *argv[]);
		void PrintHelp();
		void GlobalInit();
	
		const char *conf_path_;
		const char *log_path_;
		const char *conf_name_;

		ConfPlugin *conf_plugin_;
};
