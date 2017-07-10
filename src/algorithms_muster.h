#ifndef  ALGORITHMS_MUSTER_H_
#define  ALGORITHMS_MUSTER_H_

class BaseAlgorithms;

typedef BaseAlgorithms * (*CreateStrategy) ();

#define RegisterProcess(AlgorithmsName) \
	extern "C" \
	BaseAlgorithms * create##AlgorithmsName() \
	{return new (std::nothrow) AlgorithmsName();}

class AlgorithmsMuster
{
	public:
		std::string so_name_;
		std::string algorithms_name_;
		CreateStrategy strategy_handle_;
};


#endif
