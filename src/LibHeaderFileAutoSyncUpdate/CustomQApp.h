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
	bool release()override;
	void showIntroWindow() override;
private:
	bool initModule()override;
	bool initUi()override;
	void connectSigs()override;
private:
};
