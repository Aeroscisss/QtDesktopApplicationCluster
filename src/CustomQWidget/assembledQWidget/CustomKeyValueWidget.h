#pragma once

#include <QWidget>
#include <QFrame>
#include <QGridLayout>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>
#include <QCheckBox>
#include <QColor>

class CustomKeyValueWidget : public QWidget
{
	Q_OBJECT

public:
	enum class ValueWidgetType {
		LabelEdit = 0,
		LineEdit = 1,
		TextEdit = 2,
		CheckBox = 3,
		PathSelectButton = 4,
		LcdNumber = 5,
		ComboBox = 6
	};
	
	~CustomKeyValueWidget();
	ValueWidgetType getWidgetType();
	void setValueWidgetMinHeight(int heigth);
	void setValueWidgetMinWidth(int width);
	void setKey(QString key);
	QString getKey();
	void setKeyVisible(bool visibility);
	virtual void setStringValue(QString);
	virtual QString getStringValue();
	virtual void setEditable(bool editable);
	virtual bool getEditable();
	bool setKeyValueExpandRatio(float ratio);
	QWidget* getValueWidget();
	void setHighLight(bool isHightLighted);
	void setHighLightColor(QColor color);
signals:
	void sig_customKeyValueWidget_valueChanged();
protected:
	CustomKeyValueWidget(QWidget* parent = nullptr, ValueWidgetType type = ValueWidgetType::LabelEdit, bool editable = false, bool horizentalLayout = true);
	
	QGridLayout* gridLayout=nullptr;
	QFrame *frame_main = nullptr;
	QGridLayout* gridLayout_main = nullptr;
	QLabel* label_key = nullptr;
	QFrame* frame_value = nullptr;
	QGridLayout* gridLayout_value = nullptr;

	//Ui::CustomKeyValueWidgetClass ui;
	QWidget* currentValueWidget = nullptr;
	bool editable;
	QColor highLightColor;
	bool highLighted = false;
	bool m_horizentalLayout;
private:
	ValueWidgetType widgetType;
};
