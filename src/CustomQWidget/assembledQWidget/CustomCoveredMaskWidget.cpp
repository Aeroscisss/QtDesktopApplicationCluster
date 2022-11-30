#include "CustomCoveredMaskWidget.h"

CustomCoveredMaskWidget::CustomCoveredMaskWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.frame_main->setAutoFillBackground(true);
	setOpacity(70);
	if(parent!=nullptr)
		this->setGeometry(parent->geometry());

}

CustomCoveredMaskWidget::~CustomCoveredMaskWidget()
{
}

void CustomCoveredMaskWidget::setOpacity(int in_opacityPercent)
{
	if (in_opacityPercent > 100)
		in_opacityPercent = 100;
	if (in_opacityPercent < 0)
		return;
	float floatPercent = (float)in_opacityPercent / (float)100;
	QPalette currrPalette = qApp->palette();
	QPalette palette;
	QColor color = currrPalette.color(QPalette::ColorGroup::Normal, QPalette::ColorRole::Mid);
	color.setAlpha(floatPercent *255);
	palette.setColor(QPalette::Background, QColor(color));
	this->setPalette(palette);
	opacityPercent= in_opacityPercent;
	/*if (centralWidget != nullptr) {
		centralWidget->setPalette(palette);
	}*/
}

QWidget* CustomCoveredMaskWidget::getCentralWidget()
{
	return centralWidget;
}
void CustomCoveredMaskWidget::setSpringPreferredMode(bool isVertical)
{
	if (isVertical) {
		QSize a = ui.verticalSpacer_u->sizeHint();
		ui.verticalSpacer_u->changeSize(a.width(), a.height(), QSizePolicy::Minimum, QSizePolicy::Preferred);
		QSize b = ui.verticalSpacer_d->sizeHint();
		ui.verticalSpacer_d->changeSize(b.width(), b.height(), QSizePolicy::Minimum, QSizePolicy::Preferred);
	}
	else {
		QSize a= ui.horizontalSpacer_l->sizeHint();
		ui.horizontalSpacer_l->changeSize(a.width(), a.height(), QSizePolicy::Preferred, QSizePolicy::Minimum);
		QSize b = ui.horizontalSpacer_r->sizeHint();
		ui.horizontalSpacer_r->changeSize(b.width(), b.height(), QSizePolicy::Preferred, QSizePolicy::Minimum);
	}
}
void CustomCoveredMaskWidget::setSpringExpandingMode(bool isVertical)
{
	if (isVertical) {
		QSize a = ui.verticalSpacer_u->sizeHint();
		ui.verticalSpacer_u->changeSize(a.width(), a.height(), QSizePolicy::Minimum, QSizePolicy::Expanding);
		QSize b = ui.verticalSpacer_d->sizeHint();
		ui.verticalSpacer_d->changeSize(b.width(), b.height(), QSizePolicy::Minimum, QSizePolicy::Expanding);
	}
	else {
		QSize a = ui.horizontalSpacer_l->sizeHint();
		ui.horizontalSpacer_l->changeSize(a.width(), a.height(), QSizePolicy::Expanding, QSizePolicy::Minimum);
		QSize b = ui.horizontalSpacer_r->sizeHint();
		ui.horizontalSpacer_r->changeSize(b.width(), b.height(), QSizePolicy::Expanding, QSizePolicy::Minimum);
	}
}
void CustomCoveredMaskWidget::setCentralWidget(QWidget* widget)
{
	if (centralWidget != nullptr)
		centralWidget->deleteLater();
	widget->setParent(this);
	centralWidget = widget;
	ui.gridLayout_center->addWidget(widget);
}
