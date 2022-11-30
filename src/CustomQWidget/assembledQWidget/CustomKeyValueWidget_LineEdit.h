#pragma once
#include <QWidget>
#include <QString>
#include "CustomKeyValueWidget.h"
#include <QLineEdit>
class CustomKeyValueWidget_LineEdit : public CustomKeyValueWidget
{
	Q_OBJECT

public:
	CustomKeyValueWidget_LineEdit(QWidget* parent = nullptr, bool editable = true, bool horizentalLayout=true);
	~CustomKeyValueWidget_LineEdit();
	void setStringValue(QString)override;
	QString getStringValue()override;
	void setEditable(bool editable)override;
signals:
	void sig_customKeyValueWidget_lineEdit_textChanged(QString value);
private:
	QLineEdit* lineEdit_value = nullptr;
private slots:
	void rec_textChanged(QString text);
};
