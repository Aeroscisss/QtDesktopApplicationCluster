#pragma once
#include <QString>
#include <QJsonObject>
class FileMappingTask
{
public:
	FileMappingTask();
	FileMappingTask(QJsonObject obj);
	~FileMappingTask()=default;
	FileMappingTask(const FileMappingTask&);
	FileMappingTask(FileMappingTask&&)noexcept;
	FileMappingTask& operator=(const FileMappingTask&);
	QString name();
	void setName(QString name);
	void setScript(QString script);
	QJsonObject toJsonObj();
private:
	QString m_name;
	QString m_rawScript;
};

