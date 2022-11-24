#pragma once
/*
	 ___________________
	/					\
	|	shared widget	|
	\__________________/
*/
#ifndef QJSONIO_H
#define QJSONIO_H
//Qt
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
class QJsonIO
{
public:
	//实例公用Json接口
	static bool readJsonFile(QString filePath,QJsonDocument &doc);
	static bool convertJsonDocToObj(QJsonDocument doc, QJsonObject& obj);
	static bool writeJsonFile(QString filePath, QJsonDocument file);
};
#endif