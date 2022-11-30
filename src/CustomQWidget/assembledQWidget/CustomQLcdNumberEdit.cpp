#include "CustomQLcdNumberEdit.h"
#include "CustomQLcdNumberEdit.h"
#include "CustomQLcdNumberEdit.h"
#include "pch.h"

#include "CustomQLcdNumberEdit.h"

CustomQLcdNumberEdit::CustomQLcdNumberEdit(QWidget *parent)
	: QWidget(parent)
{
	initComponent();
}

CustomQLcdNumberEdit::~CustomQLcdNumberEdit()
{}

double CustomQLcdNumberEdit::value()
{
    return lcdNumber->value();
}

int CustomQLcdNumberEdit::intValue()
{
    return lcdNumber->intValue();
}

void CustomQLcdNumberEdit::display(double num)
{
    return lcdNumber->display(num);
}

void CustomQLcdNumberEdit::setHighLight(bool isHighLighted)
{
	if (isHighLighted) {
		frame_main->setFrameShape(QFrame::Box);
		frame_main->setFrameShadow(QFrame::Plain);
	}
	else
	{
	    frame_main->setFrameShape(QFrame::Shape::NoFrame);
		frame_main->setFrameShadow(QFrame::Shadow::Plain);
	}
	highLighted = isHighLighted;
}
bool CustomQLcdNumberEdit::eventFilter(QObject * obj, QEvent * evt)
{
    auto preValue= lcdNumber->value();
    if (obj == lcdNumber)
    {
        if (evt->type() == QEvent::MouseButtonDblClick)
        {
            lineEdit->setText(QString::number(lcdNumber->value()));
            stackedWidget->setCurrentIndex(1);
            //ui.lineEdit->setFocus();
        }
    }
    else if (obj == lineEdit)
    {
        if (evt->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyevt = static_cast<QKeyEvent*>(evt);
            if ((keyevt->key() == Qt::Key_Return) ||
                (keyevt->key() == Qt::Key_Escape) ||
                (keyevt->key() == Qt::Key_Enter))
            {
                auto afterValue=std::stod(lineEdit->text().toStdString());
                lcdNumber->display(afterValue);
                afterValue = lcdNumber->value();
                if (afterValue != preValue)
                    emit valueChanged(afterValue);
                stackedWidget->setCurrentIndex(0);
            }
        }
        else if (evt->type() == QEvent::FocusOut)
        {
            auto afterValue = std::stod(lineEdit->text().toStdString());
            lcdNumber->display(afterValue);
            afterValue = lcdNumber->value();
            if (afterValue != preValue)
                emit valueChanged(afterValue);
            stackedWidget->setCurrentIndex(0);
        }
    }

    return QWidget::eventFilter(obj, evt);
}

void CustomQLcdNumberEdit::initComponent()
{
    gridLayout_main = new QGridLayout(this);
    gridLayout_main->setContentsMargins(0, 0, 0, 0);
	frame_main = new QFrame(this);
    gridLayout_main->addWidget(frame_main);
  
    gridLayout_frame = new QGridLayout(frame_main);
	gridLayout_frame->setContentsMargins(0, 0, 0, 0);

    stackedWidget = new QStackedWidget(this);
    gridLayout_frame->addWidget(stackedWidget);

    lcdNumber= new QLCDNumber();
   // lcdNumber->setStyleSheet("background-color:white");

    lineEdit = new QLineEdit();

    lcdNumber->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    //stackedWidget->setGeometry(0, 0, 80, 80);

    lcdNumber->installEventFilter(this);
    lineEdit->installEventFilter(this);

    stackedWidget->addWidget(lcdNumber);
    stackedWidget->addWidget(lineEdit);

    stackedWidget->setCurrentWidget(lcdNumber);

    this->setMinimumHeight(20);
    stackedWidget->setMinimumHeight(20);
}
