#include "plugin_manager.h"

pthread_mutex_t pp;
pthread_mutex_t InstanceLog::m_mutex = pp;
InstanceLog *InstanceLog::m_instance = NULL;

//contrcol function
grpc::Status PluginManager::ChooseClassMethod(grpc::ServerContext *context, const request *cc, response *ss)
{
	std::string aa("wolaile");
	ss->set_result(aa);
	std::cout << cc->class_name() << std::endl;
	return grpc::Status::OK;
}

void PluginManager::PrintHelp()
{
	cout << "\33[40;35m Welcome to plugin_manager with grpc!! \33[0m" << endl;
	cout << "\33[40;35mPlease Usage:\33[0m" << endl;
	cout << "\33[40;35m-d\33[0m + config_path" << endl;
	cout << "\33[40;35m-l\33[0m + log_path" << endl;
	cout << "\33[40;35m-h\33[0m for help" << endl;
}

bool PluginManager::ParseParam(int argc, char *argv[])
{
	const char *optstring = "d:l:h";
	const struct option longopts[] = {
		{"config_path",1, NULL,'d'},
		{"log_path", 1, NULL, 'l'},
		{"help", 0, NULL, 'h'}
	};

	char c;
	while((c = getopt_long(argc, argv, optstring, longopts, NULL)) != -1)
	{
		switch(c)
		{
			case 'd':
				conf_path_ = optarg;
				break;

			case 'l':
				log_path_ = optarg;
				break;

			case 'h':
				PrintHelp();
				return false;
				break;

			default:
				return false;
				break;
		}
	}

	if( NULL == conf_path_ || NULL == log_path_)
	{
		PrintHelp();
		return false;
	}

	return true;
}

void PluginManager::InitLog()
{
	InstanceLog::GetInstance(log_path_);
}

void PluginManager::GlobalInit()
{
	InitLog();
}

bool PluginManager::Run(int argc, char *argv[])
{
	if(ParseParam(argc, argv) != true)
	{
		return false;
	}

	GlobalInit();

	return true;
}
