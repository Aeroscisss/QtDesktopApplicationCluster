#pragma once

#include <QWidget>
#include <QString>
#include "CustomKeyValueWidget.h"
#include <QCheckBox>

class CustomKeyValueWidget_CheckBox : public CustomKeyValueWidget
{
	Q_OBJECT

public:
	CustomKeyValueWidget_CheckBox(QWidget* parent = nullptr,bool editable=true,bool triState=false, bool horizentalLayout=true);
	~CustomKeyValueWidget_CheckBox();
	void setValue(int state);
	int getValue();
	void setStringValue(QString)override;
	QString getStringValue()override;
	void setEditable(bool editable)override;
	//void setIndicatorSize(QSize size);
signals:
	void sig_customKeyValueWidget_checkBox_checkStateChanged(int);
private:
	QCheckBox* checkBox_value = nullptr;
private slots:
	void rec_checkStateChanged(int);
};
