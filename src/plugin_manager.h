#ifndef CONF_MANAGER_H
#define CONF_MANAGER_H

#include<iostream>
#include <dlfcn.h>

#include "libxml/tree.h"
#include "libxml/parser.h"
#include "glog/logging.h"
#include "auto_mutex.h"
#include "algorithms_muster.h"

class Strategies
{
	public:
		std::string name_;

		std::vector<AlgorithmsMuster> algorithms_muster_;
};

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

		size_t Size() const
		{
			return strategies_.size();
		}

		const Strategies & operator [] (int id) const
		{
			return strategies_[id];
		}

	private:
		//初始化算法库
		bool InitStrategy(const char* work_path, xmlNodePtr child_node, AlgorithmsMuster *algorithms_info);
		//加载so
		void* LoadSo(std::string so_name);
		//加载函数
		void* GetFactory(std::string so_name, std::string algorithms_name);

		ConfPlugin():status_(false) {}
		~ConfPlugin() {status_ = false;}
		static ConfPlugin m_instance;
		static pthread_mutex_t mutex_;

		bool status_;
		//算法集合
		std::vector<Strategies> strategies_;
};

#endif
