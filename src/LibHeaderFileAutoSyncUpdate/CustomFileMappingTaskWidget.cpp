#include <QDialog>
#include "CustomFileMappingTaskWidget.h"
CustomFileMappingTaskWidget::CustomFileMappingTaskWidget(FileMappingTask task,QWidget *parent)
	:CustomAbstractQWidget(parent)
{
	ui.setupUi(this);
	connect(ui.textEdit_taskScript, &QTextEdit::textChanged,
		this, &CustomFileMappingTaskWidget::rec_updateTask);
	
	label_taskName = new CustomQLabelEdit(this);
	connect(label_taskName, &CustomQLabelEdit::valueChanged,
		this, &CustomFileMappingTaskWidget::rec_updateTask);
	ui.gridLayout_taskName->addWidget(label_taskName);
	label_taskName->setText(task.name());
}

CustomFileMappingTaskWidget::~CustomFileMappingTaskWidget()
{
}
FileMappingTask CustomFileMappingTaskWidget::getTask()
{
	FileMappingTask task;
	task.setName(label_taskName->text());
	task.setScript(ui.textEdit_taskScript->toPlainText());
	return task;
}
void CustomFileMappingTaskWidget::setTaskMarkNum(int num)
{
	ui.lcdNumber->display(num);
}
void CustomFileMappingTaskWidget::rec_updateTask() {
	emit sig_taskWidget_taskChanged();
}
void CustomFileMappingTaskWidget::on_btn_delTask_clicked() {

}
