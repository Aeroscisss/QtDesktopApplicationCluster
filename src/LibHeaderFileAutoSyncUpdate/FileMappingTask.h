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
	QString script();
	void setScript(QString script);
	QJsonObject toJsonObj();
	QString toConsoleString();
	QString toString();
private:
	QString m_name;
	QString m_rawScript;
};

