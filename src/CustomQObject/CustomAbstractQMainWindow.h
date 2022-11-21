#pragma once

#include <QMainWindow>

class CustomAbstractQMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	CustomAbstractQMainWindow(QWidget *parent = nullptr);
	~CustomAbstractQMainWindow();

private:
};
