#ifndef  ALGORITHMS_MUSTER_H_
#define  ALGORITHMS_MUSTER_H_

//typedef int * (*CreateStrategy) (int a, int b);
typedef std::string (*CreateStrategy) ();

class AlgorithmsMuster
{
	public:
		std::string so_name_;
		std::string algorithms_name_;
		CreateStrategy strategy_handle_;
};


#endif
