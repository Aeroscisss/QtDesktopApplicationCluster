#pragma once
#include <QWidget>
#include <QString>
#include "CustomKeyValueWidget.h"
#include "customQWidget/assembledQWidget/CustomQLcdNumberEdit.h"
class CustomKeyValueWidget_LcdNumberEdit : public CustomKeyValueWidget
{
	Q_OBJECT

public:
	CustomKeyValueWidget_LcdNumberEdit(QWidget* parent = nullptr, bool editable = true, bool horizentalLayout=true);
	~CustomKeyValueWidget_LcdNumberEdit();
	int getIntValue();
	double getValue();
	void setValue(double value);
	void setStringValue(QString)override;
	QString getStringValue()override;
	void setEditable(bool editable)override;
	CustomQLcdNumberEdit* getCustomQLcdNumberEditPtr();
signals:
	void sig_customKeyValueWidget_lcdNumberEdit_valueChanged(double value);
private:
	CustomQLcdNumberEdit* lcdNumberEdit_value = nullptr;
private slots:
	void rec_valueChanged(double value);
};
