#pragma once
#include <QString>
#include <QList>
#include <QStringList>
class FileMappingRule
{
public:
	FileMappingRule(){}
	~FileMappingRule()=default;
	FileMappingRule(const FileMappingRule&) = default;
	FileMappingRule& operator=(const FileMappingRule&) = default;
	QStringList srcList() { return srcs; }
	void setSrcList(QStringList list) { srcs = list; }
	void addSrc(QString src) { srcs.append(src); }
	QString dst() { return dstFolder; }
	void setDst(QString dst){ dstFolder = dst; }
	QStringList filterList() { return filters; }
	void setFilterList(QStringList list) { filters = list; }
	void addFilter(QString filter) { filters.append(filter); }
	bool full() {
		if (srcs.empty())
			return false;
		if (dstFolder.isEmpty())
			return false;
		if (filters.empty())
			return false;
		return true;
	}
private:
	QStringList srcs;
	QString dstFolder;
	QStringList filters;
};

