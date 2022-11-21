#pragma once

#include <QtWidgets/QMainWindow>
#include "CustomQObject/CustomAbstractQMainWindow.h"
#include "ui_CustomQMainWindow.h"

class CustomQMainWindow : public CustomAbstractQMainWindow
{
    Q_OBJECT

public:
    CustomQMainWindow(QWidget *parent = nullptr);
    ~CustomQMainWindow();
private:
    Ui::CustomQMainWindowClass ui;
};
