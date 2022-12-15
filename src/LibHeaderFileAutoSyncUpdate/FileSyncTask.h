#pragma once
#include <QString>
#include <QJsonObject>
class FileSyncTask
{
public:
	FileSyncTask();
	FileSyncTask(QJsonObject obj);
	~FileSyncTask()=default;
	FileSyncTask(const FileSyncTask&);
	FileSyncTask(FileSyncTask&&)noexcept;
	FileSyncTask& operator=(const FileSyncTask&);
	QString name();
	void setName(QString name);
	QString script();
	void setScript(QString script);
	QJsonObject toJsonObj();
	QString toConsoleString();
	QString toString();
private:
	QString m_name;
	QString m_rawScript;
};

