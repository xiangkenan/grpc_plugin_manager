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
		if(xmlStrcmp(curNode->name, BAD_CAST "strategies") != 0)
		{
			LOG(WARNING) << "it is not curNode->name";
			continue;
		}

		if(xml_kv_cmp(curNode, "switch", "off"))
		{
			continue;
		}
	}


failed:
	if(NULL != doc)
	{
		xmlFreeDoc(doc);
	}

	return status_;
}
