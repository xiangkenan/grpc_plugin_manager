#include "freq.h"
using namespace std;

RegisterProcess(FreqCtrlServerImp);

FreqCtrlServerImp::FreqCtrlServerImp(){}
FreqCtrlServerImp::~FreqCtrlServerImp(){}

bool FreqCtrlServerImp::Init()
{
	LOG(INFO) << "正在初始化...";
	return true;
}

bool FreqCtrlServerImp::Run(string& tt)
{
	LOG(INFO) << "插件架构 基本完成";
	tt = "世界你好";
	return true;
}
