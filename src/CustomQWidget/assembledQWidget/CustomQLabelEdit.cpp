#include "CustomQLabelEdit.h"

CustomQLabelEdit::CustomQLabelEdit(QWidget *parent)
	: QWidget(parent)
{
    stackedWidget = new QStackedWidget(this);

    label = new QLabel();
    label->setStyleSheet("background-color:white");

    lineEdit = new QLineEdit();

    label->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    //stackedWidget->setGeometry(0, 0, 100, 30);

    label->installEventFilter(this);
    lineEdit->installEventFilter(this);

    stackedWidget->addWidget(label);
    stackedWidget->addWidget(lineEdit);

    stackedWidget->setCurrentWidget(label);
}

CustomQLabelEdit::~CustomQLabelEdit()
{
}

void CustomQLabelEdit::setEditable(bool editable){
    _editable = editable;
}

bool CustomQLabelEdit::editable()
{
    return _editable;
}

void CustomQLabelEdit::setEnabeled(bool enable)
{
    _enable = enable;
    label->setEnabled(enable);
    lineEdit->setEnabled(enable);
}

bool CustomQLabelEdit::enabled()
{
    return _enable;
}

QString CustomQLabelEdit::text()
{
    return label->text();
}

void CustomQLabelEdit::setText(QString text)
{
    if (text != _currentText) {
        emit textChanged(text);
        emit valueChanged();
    }
    _priviousText = _currentText;
    _currentText = text;
    label -> setText(_currentText);
}

bool CustomQLabelEdit::eventFilter(QObject* obj, QEvent* evt) {
    if (!_editable)
        return QWidget::eventFilter(obj, evt);
    if (obj == label)
    {
        if (evt->type() == QEvent::MouseButtonDblClick)
        {
            lineEdit->setText(label->text());
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
                setText(lineEdit->text());
                stackedWidget->setCurrentIndex(0);
            }
        }
        else if (evt->type() == QEvent::FocusOut)
        {
            setText(lineEdit->text());
            stackedWidget->setCurrentIndex(0);
        }
    }

    return QWidget::eventFilter(obj, evt);
}
void CustomQLabelEdit::rec_textChanged(QString text) {
    emit textChanged(text);
    emit valueChanged();
}
