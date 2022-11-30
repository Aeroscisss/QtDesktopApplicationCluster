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
    /// SqliteDataBase类封装了一个常驻QSqlDataBase对象 m_database
    /// 并且对该QSqlDataBase对象进行接口封装
    /// 类内成员为封装的QSqlData对象的基本信息。包括主机名用户名密码数据库文件名路径等
    /// 本类的作为基础的Qsqlite数据库封装，提供给更高级的管理类接口进行操作，
    /// </summary>
public:
    
public:
   explicit  QSqliteDataBase(QString connectionName, QString fileName);
    ~QSqliteDataBase();//database 会被关闭
    QSqlDatabase* getQSqlDatabase();
    bool openDataBase();//打开数据库：直接尝试打开成员变量里面的数据
	bool isOpened();
	void closeDataBase();//关闭database
    bool createTable(QString tableName, QMap<QString, QString> tableData);//在database里面建立表格
    bool checkTableExist(QString tableName);//输入表格名字，在database查找表格是否存在【finished】
    bool deleteTable(QString tableName);//输入表格名字，在database里面删除表【finished】
    bool addData(QString tableName, QMap<QString, QString> tableData);//增加数据
    bool deleteData(QString tableName, QMap<QString, QString> where); //删除一条记录
    bool sendDatabaseQuery(QString m_query_sql); //发送数据库语句
    bool updateData(QString tableName, QMap<QString, QString> where, QMap<QString, QString> data);  //更新数据
    bool find(QString tableName, QList<QString> key, QMap<QString, QString> where, QList<QList<QString>>* row);    //查找
    bool find(QString tableName, QList<QString> key, QList<QList<QString>>* row);    //查找所有
    QString getError();
private:
    bool _databaseOpened = false;
	std::mutex mutex_database;
	QSqlDatabase _database;
	std::unique_ptr<QSqlQueryModel>_queryModel;//sql 查询表
	std::unique_ptr<QSqlQuery> _query;//sql 语句执行对象
	QString _databaseFilePath;   //数据库文件路径
	QSqlError _error;//sql 错误对象
	QSqlRecord _record;//sql 记录对象
	void errorReport(std::string msg);
	void infoReport(std::string msg);
	void traceReport(std::string msg);

};
