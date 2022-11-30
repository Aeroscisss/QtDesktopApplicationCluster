#pragma once
#include<QString>
class QFilePathTools
{
public:
	static bool isFile(QString path);
	static bool isDir (QString path);
	static bool exists(QString path);
	static bool isRoot(QString path);
	static bool isAbsolute(QString path); 
	static bool isReletive(QString path);
	static bool absoluteToReletive(QString absolutePath,QString & reletivePath);
	static bool reletiveToAbsolute(QString reletivePath, QString& absolutePath);
	static bool createPreFolder(QString path);
};

