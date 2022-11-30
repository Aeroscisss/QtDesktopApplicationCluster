#pragma once

#include <QDialog>
#include <QList>
#include <QString>
#include <QGridLayout>
#include <QPushButton>
#include <QFrame>
#include <mutex>
class CustomItemSelectDialogWidget : public QDialog
{
	Q_OBJECT

public:
	CustomItemSelectDialogWidget( QWidget* parent = nullptr, QString title = tr("Select a Item"));
	~CustomItemSelectDialogWidget();
	void addItem(QString text);
	void addItems(QList<QString> textList);
	void setItems(QList<QString> textList);
	void setItemButtonSize(int w=100,int h=100);
	void setItemHorizentalMaxAmount(int amount);
	int itemHorizentalMaxAmount();
	int exec(QString&text);
protected:
	QGridLayout* gridLayout_main = nullptr; 
	QFrame* frame_itemBtns = nullptr;
	QGridLayout* gridLayout_itemBtns = nullptr;
protected slots:
	void rec_itemBtnClicked();
protected:
private:
	void applyItemListToUi();
	void resetItemBtns();
	void clearItems();
private:
	QString *clickedBtnText=nullptr;
	int maxHorizentalAmount=5;
	int itemBtnWidth = 100;
	int itemBtnHeight = 100;
	struct Item {
		Item(QString in_text) {
			text = in_text;
		}
		~Item(){}
		Item(const Item& other) {
			text = other.text;
			btn = other.btn;
		}
		Item& operator=(const Item& other) {
			text = other.text;
			btn = other.btn;
		}
		QString text;
		QPushButton* btn=nullptr;
	};
	std::mutex mutex_itemList;
	QList<Item>itemList;
};
