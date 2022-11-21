#include "CustomAbstractQApp.h"

CustomAbstractQApp::CustomAbstractQApp(int& argc, char** argv)
	: QApplication(argc, argv)
{}

CustomAbstractQApp::~CustomAbstractQApp()
{
}

bool CustomAbstractQApp::initialize()
{
	if(!initModule())
		return false;
	if (!initUi())
		return false;
	return true;
}

bool CustomAbstractQApp::initModule()
{
	return false;
}

bool CustomAbstractQApp::initUi()
{
	return false;
}
