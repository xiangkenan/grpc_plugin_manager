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
	LOG(INFO) << "插件架构 基本完成";
	return true;
}
