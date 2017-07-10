#include<iostream>

#include "base_algorithms.h"

class FreqCtrlServerImp : public BaseAlgorithms
{
	public:
		FreqCtrlServerImp();
		~FreqCtrlServerImp();
		virtual bool Init();
		virtual bool Run(string& tt);
};
