#pragma once

#include <QWidget>
#include <QString>
#include "CustomKeyValueWidget.h"
#include "customQWidget/modifiedQWidget/CustomQPathSelectButton.h"
class CustomKeyValueWidget_PathSelectButton : public CustomKeyValueWidget
{
	Q_OBJECT

public:
	CustomKeyValueWidget_PathSelectButton(QWidget* parent = nullptr, bool editable = true, bool horizentalLayout=true);
	~CustomKeyValueWidget_PathSelectButton();
	void setStringValue(QString)override;
	QString getStringValue()override;
	void setPaths(QList<QString>);
	QStringList getPaths();
	void setEditable(bool editable)override;
signals:
	void sig_customKeyValueWidget_pathSelectButton_pathChanged(QList<QString> paths);
private:
	CustomQPathSelectButton* pathSelectButton_value = nullptr;
private slots:
	void rec_pathChanged(QList<QString> paths);
};
