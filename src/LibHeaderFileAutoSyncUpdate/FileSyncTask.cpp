#include "FileSyncTask.h"
#include <QJsonValue>
FileSyncTask::FileSyncTask()
{
}
FileSyncTask::FileSyncTask(QJsonObject obj)
{
	if (obj.contains("taskName")) {
		QJsonValue name = obj.value("taskName");
		if (name.isString()) {
			m_name = name.toString();
		}
	}
	if (obj.contains("rawScript")) {
		QJsonValue rawScript = obj.value("rawScript");
		if (rawScript.isString()) {
			m_rawScript = rawScript.toString();
		}
	}
}
FileSyncTask::FileSyncTask(const FileSyncTask&o)
{
	m_name = o.m_name;
	m_rawScript = o.m_rawScript;
}
FileSyncTask::FileSyncTask(FileSyncTask&& o)noexcept
{
	m_name = o.m_name;
	m_rawScript = o.m_rawScript;
}
FileSyncTask& FileSyncTask::operator=(const FileSyncTask&o)
{
	m_name = o.m_name;
	m_rawScript = o.m_rawScript;
	return *this;
}

QString FileSyncTask::name()
{
	return m_name;
}

void FileSyncTask::setName(QString name)
{
	m_name = name;
}

QString FileSyncTask::script()
{
	return m_rawScript;
}

void FileSyncTask::setScript(QString script)
{
	m_rawScript = script;
}

QJsonObject FileSyncTask::toJsonObj()
{
	QJsonObject obj;
	obj.insert("taskName", m_name);
	obj.insert("rawScript", m_rawScript);
	return obj;
}

QString FileSyncTask::toConsoleString()
{
	QString out;
	out += "----|TaskName[" + m_name + "]:\n";
	QString script;
	QStringList scriptList= m_rawScript.split("\n");
	for (int i = 0; i < scriptList.count(); i++){
		script += "     " + scriptList[i] + "\n";
	}
	out +=script;
	return out;
}

QString FileSyncTask::toString()
{
	QString out;
	out+="TaskName["+m_name+"]:";
	out += m_rawScript;
	return out;
}
