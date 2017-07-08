#ifndef CONF_MANAGER_H
#define CONF_MANAGER_H

#include<iostream>

#include "libxml/tree.h"
#include "libxml/parser.h"
#include "glog/logging.h"
#include "auto_mutex.h"

struct Strategies
{
}

class ConfPlugin
{
	public:
		static ConfPlugin* Instance()
		{
			return &m_instance;
		}

		bool Init(const char* work_path, const char* conf_file);

		//判断节点
		bool xml_kv_cmp(xmlNodePtr curNode, const std::string& key, const std::string& value);

	private:


		ConfPlugin():status_(false) {}
		~ConfPlugin() {status_ = false;}
		static ConfPlugin m_instance;
		static pthread_mutex_t mutex_;

		bool status_;

		std::string name_;
		std::string so_name_;
		std::string factory_name_;
		std::string conf_name_;

		std::vector<Strategies> strategies;
};

#endif
