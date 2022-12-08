#include "FileMappingTask.h"
#include <QJsonValue>
FileMappingTask::FileMappingTask()
{
}
FileMappingTask::FileMappingTask(QJsonObject obj)
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
FileMappingTask::FileMappingTask(const FileMappingTask&o)
{
	m_name = o.m_name;
	m_rawScript = o.m_rawScript;
}
FileMappingTask::FileMappingTask(FileMappingTask&& o)noexcept
{
	m_name = o.m_name;
	m_rawScript = o.m_rawScript;
}
FileMappingTask& FileMappingTask::operator=(const FileMappingTask&o)
{
	m_name = o.m_name;
	m_rawScript = o.m_rawScript;
	return *this;
}

QString FileMappingTask::name()
{
	return m_name;
}

void FileMappingTask::setName(QString name)
{
	m_name = name;
}

QString FileMappingTask::script()
{
	return m_rawScript;
}

void FileMappingTask::setScript(QString script)
{
	m_rawScript = script;
}

QJsonObject FileMappingTask::toJsonObj()
{
	QJsonObject obj;
	obj.insert("taskName", m_name);
	obj.insert("rawScript", m_rawScript);
	return obj;
}

QString FileMappingTask::toConsoleString()
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

QString FileMappingTask::toString()
{
	QString out;
	out+="TaskName["+m_name+"]:";
	out += m_rawScript;
	return out;
}
