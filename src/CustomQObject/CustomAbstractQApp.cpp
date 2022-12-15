#include "CustomAbstractQApp.h"

CustomAbstractQApp::CustomAbstractQApp(int& argc, char** argv)
	: QApplication(argc, argv)
{}

CustomAbstractQApp::~CustomAbstractQApp()
{
	release();
}

bool CustomAbstractQApp::initialize()
{
	return true;
}

bool CustomAbstractQApp::release()
{
	return true;
}

void CustomAbstractQApp::showIntroWindow()
{
	qDebug() << "CustomAbstractQApp::showIntroWindow ";
}

bool CustomAbstractQApp::initModule()
{
	return true;
}

bool CustomAbstractQApp::initUi()
{
	return true;
}

void CustomAbstractQApp::connectSigs()
{

}
