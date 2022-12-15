#pragma once
#include <QWidget>
#include <QDebug>
#include <vector>
#include <mutex>

class CustomAbstractQWidget : public QWidget
{
	Q_OBJECT

public:
	CustomAbstractQWidget(QWidget *parent = nullptr);
	~CustomAbstractQWidget();
protected:
	virtual void retranslate();
private:
	std::mutex mutex_subWidgetVec;
	std::vector<CustomAbstractQWidget*>subWidgetVec;//
};
