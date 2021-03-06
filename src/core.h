#include <iostream>
#include <grpc++/grpc++.h>
#include <getopt.h>

#include "ofo_main.grpc.pb.h"
#include "log.h"
#include "plugin_manager.h"
#include "base_algorithms.h"

using namespace std;

class PluginManager final : public plugins_rpc::Service
{
	public:
		grpc::Status ChooseClassMethod(const Request *cc, Response *ss);
		PluginManager():conf_path_(NULL), log_path_(NULL) {}

		bool Init(int argc, char *argv[]);
		std::unordered_map<string, std::vector<BaseAlgorithms*>> get_muster()
		{
			return instance_muster_;
		}

	private:
		bool ParseParam(int argc, char *argv[]);
		void PrintHelp();
		void GlobalInit();
		bool WorkEngine(const Strategies& strategies);
	
		const char *conf_path_;
		const char *log_path_;
		const char *conf_name_;

		//实例集合
		std::unordered_map<string, std::vector<BaseAlgorithms*>> instance_muster_;
};
