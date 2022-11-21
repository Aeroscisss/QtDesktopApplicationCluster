#include "CustomAbstractQWidget.h"

CustomAbstractQWidget::CustomAbstractQWidget(QWidget *parent)
	: QWidget(parent)
{
}

CustomAbstractQWidget::~CustomAbstractQWidget()
{
	subWidgetVec.clear();
	subWidgetVec.shrink_to_fit();
}

void CustomAbstractQWidget::retranslate()
{
	mutex_subWidgetVec.lock();
	for (auto iter = subWidgetVec.begin();iter!=subWidgetVec.end();iter++){
		(*iter)->retranslate();
	}
	mutex_subWidgetVec.unlock();
}

