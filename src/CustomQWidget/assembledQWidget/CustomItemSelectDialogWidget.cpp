#include "CustomItemSelectDialogWidget.h"
#include <algorithm>
CustomItemSelectDialogWidget::CustomItemSelectDialogWidget(QWidget *parent,QString title)
	: QDialog(parent)
{
	this->setWindowTitle(title);
	//setAttribute(Qt::WA_DeleteOnClose);
	Qt::WindowFlags flags = Qt::Dialog| Qt::WindowCloseButtonHint;
	//Qt::WindowFlags flags = Qt::Dialog | Qt::WindowCloseButtonHint| Qt::MSWindowsFixedSizeDialogHint;
	setWindowFlags(flags);

	gridLayout_main = new QGridLayout(this);

	frame_itemBtns = new QFrame(this);
	gridLayout_main->addWidget(frame_itemBtns);
	gridLayout_itemBtns = new QGridLayout(frame_itemBtns);

}

CustomItemSelectDialogWidget::~CustomItemSelectDialogWidget()
{
	clearItems();
}

void CustomItemSelectDialogWidget::addItem(QString text)
{
	mutex_itemList.lock();
	itemList.append(Item(text));
	mutex_itemList.unlock();
	applyItemListToUi();
}

void CustomItemSelectDialogWidget::addItems(QList<QString> textList)
{
	mutex_itemList.lock();
	for (auto text : textList) {
		itemList.append(Item(text));
	}
	mutex_itemList.unlock();
	applyItemListToUi();
}

void CustomItemSelectDialogWidget::setItems(QList<QString> textList)
{
	clearItems();
	addItems(textList);
}

void CustomItemSelectDialogWidget::setItemButtonSize(int w, int h)
{
	if (w < 0 || w>1000 || w < 0 || w>1000)
		return;
	itemBtnWidth = w;
	itemBtnHeight = h;
	std::lock_guard<std::mutex>locker(mutex_itemList);
	for (auto item : itemList) {
		if (item.btn != nullptr) {
			item.btn->setFixedSize(QSize(itemBtnWidth, itemBtnHeight));
		}
	}
}

void CustomItemSelectDialogWidget::setItemHorizentalMaxAmount(int amount)
{
	maxHorizentalAmount = amount;
}

int CustomItemSelectDialogWidget::itemHorizentalMaxAmount()
{
	return maxHorizentalAmount;
}

int CustomItemSelectDialogWidget::exec(QString& text)
{
	clickedBtnText=&text;
	return QDialog::exec();
}

void CustomItemSelectDialogWidget::rec_itemBtnClicked() {
	QPushButton* btn = qobject_cast<QPushButton*>(sender());//获取发射信号的对象
	if(clickedBtnText!=nullptr)
		*clickedBtnText = btn->text();//获取按钮定义的对象名称
	done(QDialog::Accepted);
}

void CustomItemSelectDialogWidget::applyItemListToUi()
{
	resetItemBtns();
	int counter = 0;
	for (auto item : itemList) {
		item.btn = new QPushButton(item.text,frame_itemBtns);
		item.btn->setFixedSize(itemBtnWidth, itemBtnHeight);
		connect(item.btn, &QPushButton::clicked,
			this, &CustomItemSelectDialogWidget::rec_itemBtnClicked);
		int hC = counter % maxHorizentalAmount;
		int vC = counter / maxHorizentalAmount;
		gridLayout_itemBtns->addWidget(item.btn, vC, hC, 1, 1);
		counter++;
	}
	setItemButtonSize();
}

void CustomItemSelectDialogWidget::resetItemBtns()
{
	std::lock_guard<std::mutex>lokcer(mutex_itemList);
	for (auto item : itemList) {
		if(item.btn!=nullptr)
			item.btn->deleteLater();
	}
}

void CustomItemSelectDialogWidget::clearItems()
{
	resetItemBtns();
	mutex_itemList.lock();
	itemList.clear();
	mutex_itemList.unlock();
}


