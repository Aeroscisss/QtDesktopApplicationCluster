#include "CustomQApp.h"
#include "CustomQMainWindow.h"
#include <QString>
#include "GlobalSettings.h"
#include "GlobalMessageRepost.h"
#include "FileSyncManager.h"
#include "FileSyncOperator.h"
#include "CustomQToolKit/ImprovedThreadControl.h"
#include <QDesktopWidget>
struct CustomAbstractQApp::AppModule
{
	GlobalSettings* globalSettings = nullptr;
	GlobalMessageRepost* globalMsgRepost = nullptr;
	FileSyncManager* fileSyncManager = nullptr;
	FileSyncOperator* fileSyncOperator = nullptr;
}appModule;
struct CustomAbstractQApp::AppUi
{
	CustomQMainWindow* mainWindow = nullptr;
}appUi;
//这是一个UTF8测试
CustomQApp::CustomQApp(int& argc, char** argv)
	:CustomAbstractQApp(argc, argv)
{
}

CustomQApp::~CustomQApp()
{
	release();
}

bool CustomQApp::initialize()
{
	if (!CustomAbstractQApp::initialize())
		return false;
	if (!initModule())
		return false;
	if (!initUi())
		return false;
	connectSigs();
	return true;
}

bool CustomQApp::release()
{
	appModule.fileSyncOperator->threadIsInterrupted = true;
	appModule.globalSettings->outputSettingFile();
	ImprovedThreadControl::stopThread(appModule.fileSyncManager->m_thread);
	return true;
}

void CustomQApp::showIntroWindow()
{
	QDesktopWidget w;

	int deskWidth = w.width();
	int deskHeight = w.height();
	//qDebug() << deskWidth;
	//qDebug() << deskHeight;

	float startX = (float)deskWidth * 0.35f;
	float startY= (float)deskWidth * 0.1f;
	float width = (float)deskWidth * 0.3f;
	float height = (float)deskHeight * 0.6f;

	appUi.mainWindow->setGeometry(QRect(startX,startY,width,height));
	appUi.mainWindow->show();

	if (!appModule.fileSyncManager->openRuleFile(GlobalSettings::Instance().reletiveLatestRuleFilePath)) {
		appModule.fileSyncManager->openRuleFile(GlobalSettings::Instance().absoluteLatestRuleFilePath);
	}
}

bool CustomQApp::initModule()
{
	try {
		appModule.globalSettings = &GlobalSettings::Instance();
		appModule.globalSettings->readSettingFile();
		appModule.globalMsgRepost = &GlobalMessageRepost::Instance();
		appModule.fileSyncManager = &FileSyncManager::Instance();
		appModule.fileSyncOperator= &FileSyncOperator::Instance();
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

