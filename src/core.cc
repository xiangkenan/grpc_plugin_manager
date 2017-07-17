#include "core.h"

pthread_mutex_t pp;
pthread_mutex_t InstanceLog::m_mutex = pp;
InstanceLog *InstanceLog::m_instance = NULL;

using google::protobuf::Map;

//contrcol function
grpc::Status PluginManager::ChooseClassMethod(const Request *cc, Response *ss)
{
	string final_result;
	Map<string, string> params = cc->params();
	if(params.count("param") <= 0 || instance_muster_.count(params["param"]) <= 0)
	{
		ss->set_result("{\"code\":-1,\"msg\":\"no param\"}");
		return grpc::Status::OK;
	}

	vector<BaseAlgorithms*> algorithms_run=instance_muster_[params["param"]];
	for(int i=0; i<algorithms_run.size(); i++)
	{
		algorithms_run[i]->Run(final_result);
	}
	ss->set_result(final_result);

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

bool PluginManager::WorkEngine(const Strategies& strategies)
{
	std::vector<AlgorithmsMuster> algorithms_muster = strategies.algorithms_muster_;
	try
	{
		for(int i=0; i<algorithms_muster.size(); i++)
		{
			CreateStrategy create = algorithms_muster[i].strategy_handle_;
			BaseAlgorithms *instance = create();
			instance_muster_[strategies.name_].push_back(instance);
			assert(instance != NULL);
			if(!instance->Init())
			{
				throw "init engine failed";
			}
		}
	}
	catch (...)
	{
		LOG(ERROR) << "init engine failed";
		exit(1);
	}
}

bool PluginManager::Init(int argc, char *argv[])
{
	if(ParseParam(argc, argv) != true)
	{
		return false;
	}

	GlobalInit();

	const ConfPlugin& strategies = *(ConfPlugin::Instance());

	for(int i=0; i<strategies.Size(); i++)
	{
		WorkEngine(strategies[i]);
	}

	return true;
}
