#include "pch.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDir>
#include <QFile>
#include <QVariant>
#include <iostream>
#include "QJsonIO.h"
//��������Ӳ�̵�Json�ļ�������JsonDoc��JsonObj
bool QJsonIO::readJsonFile(QString filePath, QJsonDocument &outDoc)
{
	//ʹ��QFile�н�filepath���ļ�
	QFile rawJsonFile(filePath);
	if (!rawJsonFile.open(QIODevice::ReadOnly))
	{
		return false;
	}
	QJsonParseError jsonParserError;
	outDoc = QJsonDocument::fromJson(rawJsonFile.readAll(), &jsonParserError);
	if (outDoc.isNull() || (jsonParserError.error != QJsonParseError::NoError)){
		return false;// �ĵ��� || jsonParserError�����д��󣬽���ʧ��
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
//������дQJsonDoc��Ӳ��
bool QJsonIO::writeJsonFile(QString filePath, QJsonDocument jsonDoc)
{
	//��������ļ����ڣ�����ձ����json���ݣ��������ձ�����ļ��ᵼ��д���λ
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
	// ��json���ı���ʽд���ļ����ر��ļ���
	rawJsonFile.write(jsonDoc.toJson());
	std::cout << "writeJsonFile, filePath" << filePath.toStdString() << std::endl;
	rawJsonFile.close();
	return true;
}