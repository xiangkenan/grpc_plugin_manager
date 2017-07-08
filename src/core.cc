#include "core.h"

pthread_mutex_t pp;
pthread_mutex_t InstanceLog::m_mutex = pp;
InstanceLog *InstanceLog::m_instance = NULL;

//contrcol function
grpc::Status PluginManager::ChooseClassMethod(grpc::ServerContext *context, const request *cc, response *ss)
{
	//Run();
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
	cout << "\33[40;35m-f\33[0m + conf_name" << endl;
	cout << "\33[40;35m-l\33[0m + log_path" << endl;
	cout << "\33[40;35m-h\33[0m for help" << endl;
}

bool PluginManager::ParseParam(int argc, char *argv[])
{
	const char *optstring = "d:f:l:h";
	const struct option longopts[] = {
		{"config_path",1, NULL,'d'},
		{"conf_name", 1, NULL, 'f'},
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

			case 'f':
				conf_name_ = optarg;
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

void PluginManager::GlobalInit()
{
	xmlKeepBlanksDefault(0);
	//init log
	InstanceLog::GetInstance(log_path_);

	//init 
	ConfPlugin* conf_plugin = ConfPlugin::Instance();
	if(NULL == conf_plugin || !conf_plugin->Init(conf_path_, conf_name_))
	{
		LOG(ERROR) << "Init plugin conf failed";
	}
}

bool PluginManager::Init(int argc, char *argv[])
{
	if(ParseParam(argc, argv) != true)
	{
		return false;
	}

	GlobalInit();

	const ConfPlugin& instance = *(ConfPlugin::Instance());

	for(int i=0; i<instance.Size(); i++)
	{
		const vector<AlgorithmsMuster>& algorithms_muster_ = instance[i].algorithms_muster_;
		CreateStrategy create = algorithms_muster_[i].strategy_handle_;
		BaseAlgorithms *instance = create();
		LOG(INFO) << "最终测试:" << instance->Run();
	}

	return true;
}
