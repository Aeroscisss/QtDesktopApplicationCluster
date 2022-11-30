#pragma once

#include <QWidget>
#include <QStackedWidget>
#include <QLCDNumber>
#include <QLineEdit>
#include <QPointer>
#include <QFrame>
#include <QKeyEvent>
#include <QGridLayout>

class CustomQLcdNumberEdit : public QWidget
{
	Q_OBJECT

public:
	CustomQLcdNumberEdit(QWidget *parent = nullptr);
	~CustomQLcdNumberEdit();
    QLCDNumber* getQLCDNumberPtr() { return lcdNumber.data(); }
    QLineEdit* getQLineEditPtr() { return lineEdit.data(); }
    double value();
    int intValue();
    void display(double num);
    void setHighLight(bool isHighLighted);
signals:
    void valueChanged(double value);
protected:
    bool eventFilter(QObject* obj, QEvent* e);
private:
    QPointer<QFrame>frame_main;
    QPointer < QGridLayout> gridLayout_main;
	QPointer < QGridLayout> gridLayout_frame;
    QPointer<QStackedWidget> stackedWidget;
    QPointer<QLCDNumber> lcdNumber;
    QPointer<QLineEdit> lineEdit;
	bool highLighted = false;
	void initComponent();
};
