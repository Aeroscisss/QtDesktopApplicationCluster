#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <qevent.h>
#include <QObject>
#include <QPointer>
class CustomQLabelEdit : public QWidget
{
	Q_OBJECT

public:
	CustomQLabelEdit(QWidget *parent = nullptr);
	~CustomQLabelEdit();
    void setEditable(bool editable);
    bool editable();
    void setEnabeled(bool enable);
	bool enabled();
    QString text();
    void setText(QString text);
signals:
    void valueChanged();
    void textChanged(QString text);
protected:
    bool eventFilter(QObject* obj, QEvent* e);
private:
    QPointer<QStackedWidget> stackedWidget;
    QPointer<QLabel> label;
    QPointer<QLineEdit> lineEdit;
    bool _editable=true;
    bool _enable = true;
    QString _priviousText;
    QString _currentText;
private slots:
    void rec_textChanged(QString);
};
