#ifndef BASE_ALGORITHMS_H_
#define BASE_ALGORITHMS_H_

#include "algorithms_muster.h"
#include "log.h"

using std::string;

class BaseAlgorithms
{
	public:
		BaseAlgorithms(){}
		virtual ~BaseAlgorithms() {}

		virtual bool Init() = 0;
		virtual bool Run(string& final_result) = 0;
};

#endif
