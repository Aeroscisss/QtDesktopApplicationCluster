#pragma once
#include <QString>
#include <QList>
#include <QMap>
#include <mutex>
#include <QJsonObject>
#include "FileSyncTask.h"
class FileSyncPattern
{
public:
	FileSyncPattern();
	FileSyncPattern(QJsonObject obj);
	FileSyncPattern(QString patternName);
	~FileSyncPattern()=default;
	FileSyncPattern(const FileSyncPattern&);
	FileSyncPattern(FileSyncPattern&&)noexcept;
	FileSyncPattern& operator=(const FileSyncPattern&);
	QList<FileSyncTask> getTaskList();
	QString name();
	void setName(QString name);
	void updateTaskList(QList<FileSyncTask>&list_task);
	void addTask(FileSyncTask task);
	void removeTask(int index);
	QJsonObject toJsonObj();
	QString toConsoleString();
	QString toString();
private:
	QString m_patternName;
	std::mutex mutex_tasks;
	QList<FileSyncTask>list_task;
};

