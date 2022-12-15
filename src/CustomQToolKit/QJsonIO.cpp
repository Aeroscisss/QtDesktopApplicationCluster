#ifdef PI_USE_PCH
#include "pch.h"
#endif
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDir>
#include <QFile>
#include <QVariant>
#include <iostream>
#include "QJsonIO.h"
//函数：度硬盘的Json文件并读入JsonDoc、JsonObj
bool QJsonIO::readJsonFile(QString filePath, QJsonDocument &outDoc)
{
	//使用QFile承接filepath的文件
	QFile rawJsonFile(filePath);
	if (!rawJsonFile.open(QIODevice::ReadOnly))
	{
		return false;
	}
	QJsonParseError jsonParserError;
	outDoc = QJsonDocument::fromJson(rawJsonFile.readAll(), &jsonParserError);
	if (outDoc.isNull() || (jsonParserError.error != QJsonParseError::NoError)){
		return false;// 文档空 || jsonParserError解析有错误，解析失败
	}
	rawJsonFile.close();
	return true;
}
bool QJsonIO::convertJsonDocToObj(QJsonDocument doc, QJsonObject& obj)
{
	if (!doc.isObject())
		return false;
	obj = doc.object();
	return true;
}
//函数：写QJsonDoc到硬盘
bool QJsonIO::writeJsonFile(QString filePath, QJsonDocument jsonDoc)
{
	//如果本身文件存在，则清空本身的json内容，如果不清空本身的文件会导致写入错位
	QFile rawJsonFile(filePath);
	if (rawJsonFile.exists())
	{
		if (!rawJsonFile.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
		{
			return false;
		}
		
	}
	else
		rawJsonFile.open(QIODevice::WriteOnly | QIODevice::Text);
	// 将json以文本形式写入文件并关闭文件。
	rawJsonFile.write(jsonDoc.toJson());
	std::cout << "writeJsonFile, filePath" << filePath.toStdString() << std::endl;
	rawJsonFile.close();
	return true;
}