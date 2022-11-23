#include "CustomQApp.h"
#include "CustomQMainWindow.h"
#include <QString>
#include "GlobalMessageRepost.h"
struct CustomAbstractQApp::AppModule 
{
	GlobalMessageRepost* globalMsgRepost = nullptr;
}appModule;
struct CustomAbstractQApp::AppUi
{
	CustomAbstractQMainWindow* mainWindow = nullptr;
}appUi;
CustomQApp::CustomQApp(int& argc, char** argv)
	:CustomAbstractQApp(argc, argv)
{
}

CustomQApp::~CustomQApp()
{
}

bool CustomQApp::initialize()
{
	return CustomAbstractQApp::initialize();
}

bool CustomQApp::initModule()
{
	try {
		appModule.globalMsgRepost = &GlobalMessageRepost::Instance();
		return true;
	}
	catch (std::exception e) {
		qDebug() << QString(e.what());
		return false;
	}
}

bool CustomQApp::initUi()
{
	try {
		appUi.mainWindow = new CustomQMainWindow();
		return true;
	}
	catch (std::exception e) {
		qDebug() << QString(e.what());
		return false;
	}
}

void CustomQApp::connectSigs()
{
	CustomAbstractQApp::connectSigs();
}
