#pragma once

#include <QMainWindow>
#include <QDebug>

class CustomAbstractQMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	CustomAbstractQMainWindow(QWidget *parent = nullptr);
	~CustomAbstractQMainWindow();

private:
};
