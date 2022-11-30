#include "QFilePathTools.h"
#include<QDir>
#include<QFileInfo>

bool QFilePathTools::isFile(QString path)
{
	return QFileInfo(path).isFile();
}

bool QFilePathTools::isDir(QString path)
{
	return QFileInfo(path).isDir();
}

bool QFilePathTools::exists(QString path)
{
	return QFileInfo(path).exists();
}

bool QFilePathTools::isRoot(QString path)
{
	return QFileInfo(path).isRoot();
}

bool QFilePathTools::isAbsolute(QString path)
{
	return QFileInfo(path).isAbsolute();
}

bool QFilePathTools::isReletive(QString path)
{
	return QFileInfo(path).isRelative();
}

bool QFilePathTools::absoluteToReletive(QString absolutePath, QString& reletivePath)
{
	QString pathString = absolutePath;
	QFileInfo fileInfo(pathString);
	if (!fileInfo.exists())
		return false;
	bool _isDir = fileInfo.isDir();
	bool _isFile = fileInfo.isFile();
	if (!(_isDir && _isFile)) {
		return false;//虚构的路径
	}
	else {
		QDir thisDir;
		reletivePath = thisDir.relativeFilePath(fileInfo.absoluteFilePath());
	}
	return true;
}

bool QFilePathTools::reletiveToAbsolute(QString reletivePath, QString& absolutePath)
{
	QString pathString = reletivePath;
	QFileInfo fileInfo(pathString);
	if (!fileInfo.exists())
		return false;
	bool _isDir = fileInfo.isDir();
	bool _isFile = fileInfo.isFile();
	if (!(_isDir && _isFile)) {
		return false;//虚构的路径
	}
	else {
		absolutePath = fileInfo.absolutePath();
	}
	return true;
}

bool QFilePathTools::createPreFolder(QString path)
{
	QString pathString = path;
	QDir dir(pathString);
	if (!dir.exists()) {
		bool ismkdir = QDir().mkdir(path + "/" + "result");
		if (!ismkdir)
			return false;
		else
			return true;
	}
	else {
		return true;
	}
}
