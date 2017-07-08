#include "freq.h"
using namespace std;

RegisterProcess(FreqCtrlServerImp);

FreqCtrlServerImp::FreqCtrlServerImp(){}
FreqCtrlServerImp::~FreqCtrlServerImp(){}

bool FreqCtrlServerImp::Init()
{
	return true;
}

bool FreqCtrlServerImp::Run()
{
	cout << "插件架构 基本完成" << endl ;
	return true;
}
