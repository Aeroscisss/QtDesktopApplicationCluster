#pragma once

#include <QWidget>
#include <QString>
#include "CustomKeyValueWidget.h"
#include "customQWidget/assembledQWidget/CustomQLabelEdit.h"
class CustomKeyValueWidget_LabelEdit : public CustomKeyValueWidget
{
	Q_OBJECT

public:
	CustomKeyValueWidget_LabelEdit(QWidget* parent = nullptr,bool editable=true,bool horizentalLayout=true);
	~CustomKeyValueWidget_LabelEdit();
	void setStringValue(QString)override;
	QString getStringValue()override;
	void setEditable(bool editable)override;
signals:
	void sig_customKeyValueWidget_labelEdit_textChanged(QString);
private:
	CustomQLabelEdit* labelEdit_value = nullptr;
private slots:
	void rec_textChanged(QString);
};
