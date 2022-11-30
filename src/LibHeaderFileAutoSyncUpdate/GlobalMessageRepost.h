#pragma once

#include <QObject>
#include <QThread>
#include <vector>
class GlobalMessageRepost  : public QObject
{
	Q_OBJECT

public:
	enum MsgDst :int {
		Console=0x01,
		MainWindowUserMsgBrowser = 0x02,
		MainWindowDevMsgBrowser = 0x04,
		MainWindowHoveredMsgBox = 0x08,
	};
	static GlobalMessageRepost& Instance();
	~GlobalMessageRepost();
	void sendNewMsg(QString msg,int enumMsgDst=0x02);
signals:
	void sig_globalMessageRepost_mainWindowDevMsgBrowser(QString msg);
	void sig_globalMessageRepost_mainWindowUserMsgBrowser(QString msg);
	void sig_globalMessageRepost_mainWindowHoveredMsgBox(QString msg);
private:
	GlobalMessageRepost();
	QThread thread_globalMessageReport;
};
