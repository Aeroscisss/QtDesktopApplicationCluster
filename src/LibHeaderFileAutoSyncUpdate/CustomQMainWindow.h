#pragma once

#include <QtWidgets/QMainWindow>
#include "CustomQObject/CustomAbstractQMainWindow.h"
#include "CustomQWidget/CustomMsgBrowserWidget.h"
#include "ui_CustomQMainWindow.h"

class CustomQMainWindow : public CustomAbstractQMainWindow
{
    Q_OBJECT

public:
    CustomQMainWindow(QWidget *parent = nullptr);
    ~CustomQMainWindow();
public slots:
    
private:
    Ui::CustomQMainWindowClass ui;
    CustomMsgBrowserWidget* msgBrowser;
    void connectSigs();
private slots:
    void on_action_createNewPattern_triggered();
};
