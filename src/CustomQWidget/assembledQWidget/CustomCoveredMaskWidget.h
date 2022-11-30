#pragma once

#include <QWidget>
#include "ui_CustomCoveredMaskWidget.h"
#include <QGraphicsOpacityEffect>
class CustomCoveredMaskWidget : public QWidget
{
	Q_OBJECT

public:
	CustomCoveredMaskWidget(QWidget *parent = nullptr);
	~CustomCoveredMaskWidget();
	void setOpacity(int opacityPercent);
	void setCentralWidget(QWidget * widget);
	QWidget * getCentralWidget();
	void setSpringExpandingMode(bool isVertical);
	void setSpringPreferredMode(bool isVertical);

private:
	Ui::CustomCoveredMaskWidgetClass ui;
	QWidget* centralWidget = nullptr;
	int opacityPercent;
};
