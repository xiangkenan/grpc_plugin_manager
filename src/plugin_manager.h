#ifndef CONF_MANAGER_H
#define CONF_MANAGER_H

#include<iostream>

class ConfPlugin
{
	public:
		static ConfPlugin* Instance()
		{
			return &m_instance;
		}

		bool Init(const std::string& work_path, const std::string& conf_file);

	private:

		static ConfPlugin m_instance;
};

#endif
