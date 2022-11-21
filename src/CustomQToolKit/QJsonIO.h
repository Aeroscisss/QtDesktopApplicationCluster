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
	//ʵ������Json�ӿ�
	static bool readJsonFile(QString filePath,QJsonDocument &doc);
	static bool convertJsonDocToObj(QJsonDocument doc, QJsonObject& obj);
	static bool writeJsonFile(QString filePath, QJsonDocument file);
};
#endif