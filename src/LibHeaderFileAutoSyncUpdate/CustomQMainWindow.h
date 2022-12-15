#pragma once

#include <QtWidgets/QMainWindow>
#include <QStackedWidget>
#include <QMap>
#include <QString>
#include "CustomQObject/CustomAbstractQMainWindow.h"
#include "CustomQWidget/CustomMsgBrowserWidget.h"
#include "CustomFileSyncPatternWidget.h"
#include "ui_CustomQMainWindow.h"

class CustomQMainWindow : public CustomAbstractQMainWindow
{
    Q_OBJECT

public:
    CustomQMainWindow(QWidget *parent = nullptr);
    ~CustomQMainWindow();
public slots:
    void rec_resetPatterns();
    void rec_refreshPatterns();
    void rec_refreshPatternContents();
    void rec_refreshSaveAsAction();
    void rec_updateTitle();
signals:
    void sig_requestOpenRuleFile(QString filePath);
    void sig_requestCloseCurrRuleFile();
    void sig_requestSaveRuleFile(QString filePath);
    void sig_requestCreateNewPattern(QString patternName);
    void sig_requestDeletePattern(QString patternName);
    void sig_requestPrintPatternsToConsole();
private:
    Ui::CustomQMainWindowClass ui;
    void connectSigs();
    void refreshPatternComboBox();
    void refreshPatternContents();
private:
    CustomMsgBrowserWidget* msgBrowser = nullptr;
    QStackedWidget* stackedWidget_pattern = nullptr;
    QMap<QString ,CustomFileSyncPatternWidget*>map_patternWidget;
    CustomFileSyncPatternWidget* currentPatternWidget = nullptr;
private slots:
    void on_action_openFile_triggered();
    void on_action_saveFile_triggered();
    void on_action_saveFileAs_triggered();
    void on_action_closeFile_triggered();
    void on_action_close_triggered();
    void on_action_createNewPattern_triggered();
    void on_action_delCurrPattern_triggered();
    void on_action_printPatternsToConsole_triggered();
    void on_btn_apply_clicked();
};
