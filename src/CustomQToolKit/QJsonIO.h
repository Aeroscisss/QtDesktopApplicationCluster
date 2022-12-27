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
class QJsonIO:public QObject
{
public:
	//实例公用Json接口
	bool readJsonFile(QString filePath,QJsonDocument &doc);
	bool convertJsonDocToObj(QJsonDocument doc, QJsonObject& obj);
	bool writeJsonFile(QString filePath, QJsonDocument file);
	QString errorMsg;
};
#endif