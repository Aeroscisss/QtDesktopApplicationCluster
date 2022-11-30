#pragma once

#include <QtWidgets/QMainWindow>
#include "CustomQObject/CustomAbstractQMainWindow.h"
#include "CustomQWidget/CustomMsgBrowserWidget.h"
#include "CustomFileMappingPatternWidget.h"
#include "ui_CustomQMainWindow.h"

class CustomQMainWindow : public CustomAbstractQMainWindow
{
    Q_OBJECT

public:
    CustomQMainWindow(QWidget *parent = nullptr);
    ~CustomQMainWindow();
public slots:
    void rec_refreshPatterns();
    void rec_refresPatternContents();
signals:
    void sig_requestCreateNewPattern(QString patternName);
    void sig_requestDeletePattern(QString patternName);
private:
    Ui::CustomQMainWindowClass ui;
    void connectSigs();
    void refreshPatternComboBox();
    void refreshPatternContents();
private:
    CustomMsgBrowserWidget* msgBrowser = nullptr;
    std::unique_ptr<CustomFileMappingPatternWidget> patternWidget=nullptr;
private slots:
    void on_action_createNewPattern_triggered();
    void on_action_delCurrPattern_triggered();
};
