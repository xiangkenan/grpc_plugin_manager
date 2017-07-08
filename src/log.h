#ifndef LOG_H_
#define LOG_H_

#include<iostream>

#include "glog/logging.h"

using namespace std;

class InstanceLog
{
	public:
		static void GetInstance(string log_path)
		{
			pthread_mutex_lock(&m_mutex);
			if(m_instance == NULL)
			{
				m_instance = new InstanceLog(log_path);
			}
			pthread_mutex_unlock(&m_mutex);
		}
	private:
		InstanceLog(string log_path)
		{
			google::InitGoogleLogging("plugin_manager");
			FLAGS_logtostderr = false;
			FLAGS_log_dir=log_path;
			FLAGS_max_log_size = 2000;
			FLAGS_logbufsecs = 0;
		}

		static pthread_mutex_t m_mutex;
		static InstanceLog *m_instance;
};

#endif
