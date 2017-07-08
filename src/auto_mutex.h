#ifndef AUTO_MUTEX_H_
#define AUTO_MUTEX_H_

class AutoMutex
{
	public:
		explicit AutoMutex(pthread_mutex_t &mutex): mutex_(mutex)
		{
			pthread_mutex_lock(&mutex_);
		}

		~AutoMutex() 
		{
			pthread_mutex_unlock(&mutex_);
		}

	private:
		pthread_mutex_t & mutex_;
};

#endif
