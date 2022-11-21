#pragma once

#include <QApplication>
#include <memory>
#include "CustomQObject/CustomAbstractQApp.h"
class CustomQApp  : public CustomAbstractQApp
{
	Q_OBJECT

public:
	CustomQApp(int& argc, char** argv);
	~CustomQApp();
	bool initialize()override;
private:
	bool initModule()override;
	bool initUi()override;
};
