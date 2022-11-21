#include "CustomQApp.h"
#include "CustomQMainWindow.h"
#include <QString>
CustomQApp::CustomQApp(int& argc, char** argv)
	:CustomAbstractQApp(argc, argv)
{
}

CustomQApp::~CustomQApp()
{
}

bool CustomQApp::initialize()
{
	//return false;
	if (!initModule())
		return false;
	if (!initUi())
		return false;
	return true;
}

bool CustomQApp::initModule()
{
	try {
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
		mainWindow = new CustomQMainWindow();
		return true;
	}
	catch (std::exception e) {
		qDebug() << QString(e.what());
		return false;
	}
}
