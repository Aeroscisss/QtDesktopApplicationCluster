#pragma once

#include <QApplication>
#include <memory>
#include <QPointer>
#include <QDebug>
#include "CustomAbstractQMainWindow.h"
class CustomAbstractQApp  : public QApplication
{
	Q_OBJECT
public:
	CustomAbstractQApp(int& argc, char** argv);
	~CustomAbstractQApp();
	virtual bool initialize();
	virtual bool release();
	virtual void showIntroWindow();
public:
protected:
	virtual bool initModule();
	virtual bool initUi();
	virtual void connectSigs();
	struct AppModule;
	struct AppUi;
private:
};
