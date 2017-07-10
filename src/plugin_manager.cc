#include"plugin_manager.h"

using std::string;
using std::cout;

ConfPlugin ConfPlugin::m_instance;
pthread_mutex_t ConfPlugin::mutex_ = PTHREAD_MUTEX_INITIALIZER;

bool ConfPlugin::xml_kv_cmp(xmlNodePtr curNode, const string& key, const string& value)
{
	bool flag = false;
	xmlChar * szAttr = xmlGetProp(curNode, BAD_CAST "switch");
	if(NULL != szAttr)
	{
		if(xmlStrcmp(szAttr, BAD_CAST value.c_str()) == 0)
		{
			flag = true;
		}
		xmlFree(szAttr);
	}
	return flag;
}

bool ConfPlugin::Init(const char* work_path, const char* conf_file)
{
	if(status_)
	{
		return true;
	}

	if(work_path == NULL || conf_file == NULL)
	{
		LOG(ERROR) << "param error work_path conf_file!";
		return false;
	}

	AutoMutex auto_mutex(mutex_);
	if(status_)
	{
		return true;
	}

	std::string filename(work_path);
	filename += conf_file;

	xmlDocPtr doc = xmlReadFile(filename.c_str(), "GB2312", XML_PARSE_RECOVER);
	if(NULL == doc)
	{
		LOG(ERROR) << "xmlReadFile: " << filename << " error!";
		return false;
	}

	xmlNodePtr curNode = xmlDocGetRootElement(doc);
	if(NULL == curNode)
	{
		LOG(ERROR) << "xmlDocGetRootElement error!";
		goto failed;
	}

	if(xmlStrcmp(curNode->name, BAD_CAST "plugins_manager") != 0)
	{
		LOG(ERROR) << curNode->name << " is not <plugins_manager>";
		goto failed;
	}

	for(curNode = curNode->xmlChildrenNode; curNode != NULL; curNode = curNode->next)
	{
		Strategies strategies;

		if(xmlStrcmp(curNode->name, BAD_CAST "strategies") != 0)
		{
			LOG(WARNING) << "it is not strategies";
			continue;
		}

		if(xml_kv_cmp(curNode, "switch", "off"))
		{
			continue;
		}

		xmlChar * szAttr = xmlGetProp(curNode, BAD_CAST "name");
		if(NULL != szAttr)
		{
			strategies.name_ = (char *)szAttr;
			xmlFree(szAttr);
		}

		xmlNodePtr child_node;
		for(child_node = curNode->xmlChildrenNode; child_node != NULL; child_node = child_node->next)
		{
			if(xmlStrcmp(child_node->name, BAD_CAST "strategy") != 0)
			{
				LOG(WARNING) << "it is not strategy";
				continue;
			}

			if(xml_kv_cmp(child_node, "switch", "off"))
			{
				LOG(INFO) << (char *)xmlGetProp(child_node, BAD_CAST "name") << ": is off";
				continue;
			}

			AlgorithmsMuster algorithms_info;
			if(!InitStrategy(work_path, child_node, &algorithms_info))
			{
				goto failed;
			}

			strategies.algorithms_muster_.push_back(algorithms_info);
		}

		strategies_.push_back(strategies);
	}

	status_ = true;

failed:
	if(NULL != doc)
	{
		xmlFreeDoc(doc);
	}

	return status_;
}

bool ConfPlugin::InitStrategy(const char* work_path, xmlNodePtr child_node, AlgorithmsMuster *algorithms_info)
{
	xmlChar *szAttr = xmlGetProp(child_node, BAD_CAST "name");
	if(szAttr != NULL)
	{
		algorithms_info->algorithms_name_ = "create";
		algorithms_info->algorithms_name_.append( (char *)szAttr);
		xmlFree(szAttr);
	}
	else
	{
		return false;
	}

	szAttr = xmlGetProp(child_node, BAD_CAST "so");
	if(szAttr != NULL)
	{
		algorithms_info->so_name_ = work_path;
		algorithms_info->so_name_ += (char *)szAttr;
		xmlFree(szAttr);
	}
	else
	{
		return false;
	}

	algorithms_info->strategy_handle_ = (CreateStrategy)
		GetFactory(algorithms_info->so_name_, algorithms_info->algorithms_name_);

	if(algorithms_info->strategy_handle_ == NULL)
	{
		return false;
	}

	return true;
}

void* ConfPlugin::LoadSo(string so_name)
{
	if(so_name == "")
	{
		return NULL;
	}
	
	void* handle;
	handle = dlopen(so_name.c_str(), RTLD_LAZY);
	const char* dlopen_err = dlerror();
	if(dlopen_err != NULL)
	{
		LOG(WARNING) << "so load:" << so_name << " error: " << dlopen_err;
	}
	
	return handle;
}

void* ConfPlugin::GetFactory(string so_name, string algorithms_name)
{
	void* handle = LoadSo(so_name);;
	void* factory = NULL;
	if(handle != NULL)
	{
		 factory = dlsym(handle, algorithms_name.c_str());
		 if(NULL == factory)
		 {
		 	LOG(ERROR) << "load so: " << so_name << " error:" << dlerror();
		 }
	}

	return factory;
}
