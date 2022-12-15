#pragma once
#include <QString>
#include <QList>
#include <QStringList>
class FileSyncRule
{
public:
	FileSyncRule(){}
	~FileSyncRule()=default;
	FileSyncRule(const FileSyncRule&) = default;
	FileSyncRule& operator=(const FileSyncRule&) = default;
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
	std::string toString() {
		QString result;
		result += "Rule:\n";
		result += "Src:{\n";
		for (QString src : srcs) {
			result += "  " + src + "\n";
		}
		result += "}\n";
		result += "Dst:";
		result += dstFolder+"\n";
		result += "filters:{\n";
		for (QString filter : filters) {
			result += "  " + filter + "\n";
		}
		result += "}\n";
	}
private:
	QStringList srcs;
	QString dstFolder;
	QStringList filters;
};

