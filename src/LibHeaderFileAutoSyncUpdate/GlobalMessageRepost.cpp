#include "GlobalMessageRepost.h"
#include <memory>
#include <iostream>
using namespace std;
GlobalMessageRepost& GlobalMessageRepost::Instance() {
	static std::unique_ptr<GlobalMessageRepost>instance_ptr =
		std::unique_ptr<GlobalMessageRepost>(new GlobalMessageRepost());
	return *instance_ptr;
}

GlobalMessageRepost::GlobalMessageRepost()
{
	this->moveToThread(&thread_globalMessageReport);
	thread_globalMessageReport.start();
}

GlobalMessageRepost::~GlobalMessageRepost()
{
	thread_globalMessageReport.requestInterruption();
	std::this_thread::sleep_for(chrono::milliseconds(10));
	thread_globalMessageReport.quit();
	thread_globalMessageReport.wait();
}
void GlobalMessageRepost::sendNewMsg(QString msg, int enumMsgDst)
{
	if (enumMsgDst >0xFF||enumMsgDst<0)
	{
		std::cout << "GlobalMessageRepost::sendNewMsg Fail,No Such Msg Dst,Please Check." << endl;
		return;
	}
	for (int n = 0; n < 4;n++) {
		int mask = 0x01 << n;
		int dst =mask&enumMsgDst;
		switch (dst) {
		case MsgDst::Console:
			msg += "\n";
			printf_s(msg.toStdString().c_str());
			break;
		case MsgDst::MainWindowUserMsgBrowser:
			emit sig_globalMessageRepost_mainWindowUserMsgBrowser(msg);
			//cout << "emit sig_globalMessageRepost_mainWindowUserMsgBrowser(msg);" << endl;
			break;
		case MsgDst::MainWindowDevMsgBrowser:
			emit sig_globalMessageRepost_mainWindowDevMsgBrowser(msg);
			//cout << "emit sig_globalMessageRepost_mainWindowDevMsgBrowser(msg);" << endl;
			break;
		case MsgDst::MainWindowHoveredMsgBox:
			emit sig_globalMessageRepost_mainWindowHoveredMsgBox(msg);
			//cout << "emit sig_globalMessageRepost_mainWindowHoveredMsgBox(msg);" << endl;
			break;
		default:
			break;
		}
	}
}