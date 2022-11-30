#pragma once

#include <QWidget>
#include <QString>
#include "CustomKeyValueWidget.h"
#include <QComboBox>
#include <QList>
class CustomKeyValueWidget_ComboBox : public CustomKeyValueWidget
{
	Q_OBJECT

public:
	CustomKeyValueWidget_ComboBox(QWidget* parent = nullptr, bool editable = true,bool horizentalLayout=true);
	~CustomKeyValueWidget_ComboBox();
	void setItemList(QList<QString>list);
	void setStringValue(QString string)override;
	QString getStringValue()override;
	void setEditable(bool editable)override;
signals:
	void sig_customKeyValueWidget_comboBox_textChanged(QString text);
private:
	QComboBox* comboBox_value = nullptr;
private slots:
	void rec_valueChanged(int);
};
