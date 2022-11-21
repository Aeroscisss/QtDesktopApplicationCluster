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
public:
	QPointer<CustomAbstractQMainWindow> mainWindow=nullptr;
protected:
	virtual bool initModule();
	virtual bool initUi();
private:
};
