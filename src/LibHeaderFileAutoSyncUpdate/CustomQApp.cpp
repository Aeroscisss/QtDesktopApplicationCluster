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
	if(!CustomAbstractQApp::initialize())
		return false;
	if(!initModule())
		return false;
	if (!initUi())
		return false;
	return true;
}

bool CustomQApp::release()
{
	return true;
}

void CustomQApp::showIntroWindow()
{
	appUi.mainWindow->show();
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
