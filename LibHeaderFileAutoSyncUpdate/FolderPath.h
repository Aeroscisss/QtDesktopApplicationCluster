#pragma once
#include <QString>
#include <QFileInfo>
class FolderPath
{
public:
	FolderPath(QString path);
	~FolderPath();
	FolderPath(const FolderPath&)=default;
	FolderPath(FolderPath&&) = default;
	FolderPath& operator=(const FolderPath&);
	FolderPath& operator=(FolderPath&&);
	bool valid();
private:
	QFileInfo folderInfo;
	QString folderPathString;
};

