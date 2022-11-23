#pragma once
#include <QString>
#include <QList>
class FileMappingUnit
{
public:
	FileMappingUnit();
	~FileMappingUnit();
	bool valid();
	void setValid(bool valid);
private:
	bool m_valid = false;
	QString m_srcFolderPath;
	QList<QString>m_dstFolderPaths;
};

