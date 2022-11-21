#pragma once

#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QMap>
#include <QDebug>
#include <QMessageBox>
#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <mutex>

class QSqliteDataBase 
{
    /// <summary>
    /// SqliteDataBase���װ��һ����פQSqlDataBase���� m_database
    /// ���ҶԸ�QSqlDataBase������нӿڷ�װ
    /// ���ڳ�ԱΪ��װ��QSqlData����Ļ�����Ϣ�������������û����������ݿ��ļ���·����
    /// �������Ϊ������Qsqlite���ݿ��װ���ṩ�����߼��Ĺ�����ӿڽ��в�����
    /// </summary>
public:
    
public:
   explicit  QSqliteDataBase(QString connectionName, QString fileName);
    ~QSqliteDataBase();//database �ᱻ�ر�
    QSqlDatabase* getQSqlDatabase();
    bool openDataBase();//�����ݿ⣺ֱ�ӳ��Դ򿪳�Ա�������������
	bool isOpened();
	void closeDataBase();//�ر�database
    bool createTable(QString tableName, QMap<QString, QString> tableData);//��database���潨�����
    bool checkTableExist(QString tableName);//���������֣���database���ұ���Ƿ���ڡ�finished��
    bool deleteTable(QString tableName);//���������֣���database����ɾ����finished��
    bool addData(QString tableName, QMap<QString, QString> tableData);//��������
    bool deleteData(QString tableName, QMap<QString, QString> where); //ɾ��һ����¼
    bool sendDatabaseQuery(QString m_query_sql); //�������ݿ����
    bool updateData(QString tableName, QMap<QString, QString> where, QMap<QString, QString> data);  //��������
    bool find(QString tableName, QList<QString> key, QMap<QString, QString> where, QList<QList<QString>>* row);    //����
    bool find(QString tableName, QList<QString> key, QList<QList<QString>>* row);    //��������
    QString getError();
private:
    bool _databaseOpened = false;
	std::mutex mutex_database;
	QSqlDatabase _database;
	std::unique_ptr<QSqlQueryModel>_queryModel;//sql ��ѯ��
	std::unique_ptr<QSqlQuery> _query;//sql ���ִ�ж���
	QString _databaseFilePath;   //���ݿ��ļ�·��
	QSqlError _error;//sql �������
	QSqlRecord _record;//sql ��¼����
	void errorReport(std::string msg);
	void infoReport(std::string msg);
	void traceReport(std::string msg);

};
