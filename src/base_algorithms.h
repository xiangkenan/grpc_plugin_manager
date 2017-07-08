#ifndef BASE_ALGORITHMS_H_
#define BASE_ALGORITHMS_H_

#include "algorithms_muster.h"

class BaseAlgorithms
{
	public:
		BaseAlgorithms(){}
		virtual ~BaseAlgorithms() {}

		virtual bool Init() = 0;
		virtual bool Run() = 0;
};

#endif
