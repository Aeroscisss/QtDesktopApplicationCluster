#include <QDialog>
#include "CustomFileSyncTaskWidget.h"
CustomFileSyncTaskWidget::CustomFileSyncTaskWidget(FileSyncTask task, int taskNum,QWidget *parent)
	:CustomAbstractQWidget(parent),m_taskNum(taskNum)
{
	ui.setupUi(this);
	connect(ui.textEdit_taskScript, &QTextEdit::textChanged,
		this, &CustomFileSyncTaskWidget::rec_updateTask);
	label_taskName = new CustomQLabelEdit(this);
	label_taskName->setText(task.name()); 
	connect(label_taskName, &CustomQLabelEdit::valueChanged,
		this, &CustomFileSyncTaskWidget::rec_updateTask);
	ui.gridLayout_taskName->addWidget(label_taskName);
	ui.textEdit_taskScript->setText(task.script());
}
CustomFileSyncTaskWidget::~CustomFileSyncTaskWidget()
{
}
void CustomFileSyncTaskWidget::setTaskMarkNum(int num){
	ui.lcdNumber->display(num);
}
int CustomFileSyncTaskWidget::taskMarkNum()
{
	return m_taskNum;
}
void CustomFileSyncTaskWidget::rec_updateTask() {
	task.setName(label_taskName->text());
	task.setScript(ui.textEdit_taskScript->toPlainText());
	emit sig_taskWidget_taskChanged();
}
void CustomFileSyncTaskWidget::on_btn_delTask_clicked() {
	emit sig_taskWidget_requestDelete();
}
