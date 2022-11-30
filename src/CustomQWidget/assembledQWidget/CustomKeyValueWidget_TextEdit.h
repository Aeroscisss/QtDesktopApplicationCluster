#pragma once

#include <QWidget>
#include <QString>
#include "CustomKeyValueWidget.h"
#include <QTextEdit>
class CustomKeyValueWidget_TextEdit : public CustomKeyValueWidget
{
	Q_OBJECT

public:
	CustomKeyValueWidget_TextEdit(QWidget* parent = nullptr, bool editable = true, bool horizentalLayout=true);
	~CustomKeyValueWidget_TextEdit();
	void setStringValue(QString)override;
	QString getStringValue()override;
	void setEditable(bool editable)override;
signals:
	void sig_customKeyValueWidget_textEdit_textChanged(QString text);
private:
	QTextEdit* textEdit_value = nullptr;
private slots:
	void rec_textChanged();
};
