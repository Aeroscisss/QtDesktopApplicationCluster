#include "pch.h"
#include "QSqliteDataBase.h"
#include "PiStandaloneReport.h"
#include "basicModule/DevelopLog.h"
#include <QObject>
//std::map<QSqliteDataBase::DataType, std::string>QSqliteDataBase::SqliteDataTypeStringMap = {
//    {DataType::Type_Null,"NULL"},
//    {DataType::Type_Integer,"INTEGER"},
//    {DataType::Type_Real,"REAL"},
//    {DataType::Type_Text,"TEXT"},
//    {DataType::Type_Blob,"BLOB"},
//};
//std::map< QSqliteDataBase::ConstraintType, std::string>QSqliteDataBase::SqliteConstraintTypeStringMap = {
//    {ConstraintType::NotNull,"NOT NULL"},
//    {ConstraintType::Deafault,"DEAFAULT"},
//    {ConstraintType::Unique,"UNIQUE"},
//    {ConstraintType::PrimaryKey,"PRIMARY KEY"},
//    {ConstraintType::Check,"CHECK"}
//};

//构造函数：参数为连接名称，主机名，用户名，密码
QSqliteDataBase::QSqliteDataBase(QString connectionName, QString fileName)
{
    if (QSqlDatabase::contains(connectionName))
        _database = QSqlDatabase::database(connectionName);
    else
        _database =QSqlDatabase::addDatabase("QSQLITE", connectionName);
    _databaseFilePath = fileName;
    _query = std::unique_ptr<QSqlQuery>(new QSqlQuery(_database));
    _database.setDatabaseName(_databaseFilePath);
}
//析构函数，关闭数据库
QSqliteDataBase::~QSqliteDataBase()
{
    closeDataBase();
}
QSqlDatabase* QSqliteDataBase::getQSqlDatabase()
{
    return &_database;
}
//打开数据库：本函数默认打开内构QSqlDataBase
bool QSqliteDataBase::openDataBase()
{
    std::lock_guard<std::mutex>locker(mutex_database);
    if (!_database.open()){
        _error = _database.lastError();
        DevelopLog::error("fail open sqlite database " + _database.connectionName().toStdString()+" error:"+_error.databaseText().toStdString(), DevelopLog::DevelopLogDst::GeneralDailyFile, true);
        QMessageBox::warning(nullptr, QObject::tr("Warning"), _database.lastError().text());//无法打开数据库，向主界面发送错误信息
        return false;//打开失败
    }
    else
        _databaseOpened = true;
    return true;//打开成功
}
//获取数据库是否已经打开
bool QSqliteDataBase::isOpened()
{
    return _databaseOpened;
}

//关闭数据库，此函数默认关闭本类内的内构QSqlDataBase
void  QSqliteDataBase::closeDataBase()
{
    std::lock_guard<std::mutex>locker(mutex_database);
    _database.close();
    _databaseOpened = false;
}
//输入表格名字，在database查找表格是否存在，返回bool值
bool QSqliteDataBase::checkTableExist(QString tableName)
{
    QStringList a=_database.tables();
    if (!isOpened())
        return false;
    std::lock_guard<std::mutex>locker(mutex_database);
    QString sql;
    sql = "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name ='" + tableName + "'";//是否已经存在表car_bayonet_info
    _query->prepare(sql);
    sql += ";";
    if (!_query->exec())
        return false;
    return true;
}

//删除表格，此函数根据给定的表格名称的QString删除掉对应的表格
bool QSqliteDataBase::deleteTable(QString tableName)//删除表格【finished】
{
	if (!isOpened())
		return false; 
    std::lock_guard<std::mutex>locker(mutex_database);
    //检测表格存在，不存在就写入错误信息并返回
    if (checkTableExist(tableName)){
        QString error = QString::fromUtf8("删除表格") + tableName + QString::fromUtf8("失败，表格不存在或不完整");
        _error = error;
        return false;//执行失败
    }
    //表格存在，尝试进行表格删除操作
    QString sql;
    sql = "DROP TABLE " + tableName;
    sql += ";";
    _query->prepare(sql);
    if (_query->exec())
        return true;//执行成功
    else{
        QString error = QString::fromUtf8("删除表格") + tableName + QString::fromUtf8("失败，原因未知");
        _error = error;
        _error = _database.lastError();
        return false;//执行失败
    }
}

//【finished】创建表格，该函数给内构的QSqlDataBase类添加一个表格。Qmap类的tabeldata内的pair的格式为名称-数据类型
bool  QSqliteDataBase::createTable(QString tableName, QMap<QString, QString> tableData)
{
	if (!isOpened())
		return false;
	std::lock_guard<std::mutex>locker(mutex_database);
    QString sql = "create table " + tableName + " (";
    for (QMap<QString, QString>::const_iterator i = tableData.constBegin(); i != tableData.constEnd(); i++)
    {
        sql += i.key() + ' ' + i.value();
        if (i != tableData.constEnd())
            sql += ',';
    }
    sql.chop(1);
    sql += ")";
    sql += ";";
    _query->prepare(sql);
    int result = _query->exec();
	if (!result) {
		_error = _query->lastError();
		errorReport("QSqliteDataBase::createTable出错,错误代码:" + std::to_string((int)_error.type()));
	}
    return  result;
}


//【finished】向指定的table里面添加数据，输入表格的名字
bool  QSqliteDataBase::addData(QString tableName, QMap<QString, QString> tableData)//增加数据
{
	if (!isOpened())
		return false;
	std::lock_guard<std::mutex>locker(mutex_database);
    QString sql = "insert into " + tableName + "(";
    QString values = " values(";
    for (QMap<QString, QString>::const_iterator i = tableData.constBegin(); i != tableData.constEnd(); i++)
    {
        sql += i.key() + ", ";
        values += i.value() + ", ";
    }
    sql.chop(2);
    values.chop(2);
    sql += ")";
    values += ")";
    sql += values;
    sql += ";";
    qDebug() << sql;
    _query->prepare(sql);
    int result = _query->exec();
    if (!result){
        _error = _query->lastError();
        errorReport("QSqliteDataBase::addData出错,错误代码:"+std::to_string((int)_error.type()));
    }
    return true;
}


//【finished】在指定的table里面删除数据，输入表格的名字和记录
bool QSqliteDataBase::deleteData(QString tableName, QMap<QString, QString> where)//删除一条记录
{
	if (!isOpened())
		return false;
	std::lock_guard<std::mutex>locker(mutex_database);
    QString sql = "delete from ";
    sql += tableName;
    sql += " where ";
    for (QMap<QString, QString>::const_iterator i = where.constBegin(); i != where.constEnd(); i++)
    {
        sql += i.key() + "=";
        sql += "'" + i.value() + "' ";
    }
    sql.chop(2);
    sql += ";";
    _query->prepare(sql);
    int result = _query->exec();
	if (!result) {
		_error = _query->lastError();
		errorReport("QSqliteDataBase::deleteData出错,错误代码:" + std::to_string((int)_error.type()));
	}
    return result;
}

//发送数据库语句
bool QSqliteDataBase::sendDatabaseQuery(QString _query_sql)
{
	if (!isOpened())
		return false;
	std::lock_guard<std::mutex>locker(mutex_database);
    int pos = _query_sql.size() - 1;
    QChar last = _query_sql.at(pos);
    if(last ==";")
        _query->prepare(_query_sql);
    else
    {
        _query_sql += ";";
        _query->prepare(_query_sql);
    }
    int result = _query->exec();
	if (!result) {
		_error = _query->lastError();
		errorReport("QSqliteDataBase::sendDatabaseQuery出错,错误代码:" + std::to_string((int)_error.type()));
	}
    return result;
}

//更新数据
bool QSqliteDataBase::updateData(QString tableName, QMap<QString, QString> where, QMap<QString, QString> data)
{
	if (!isOpened())
		return false;
	std::lock_guard<std::mutex>locker(mutex_database);
    QString sql = "update " + tableName + " set";
    for (QMap<QString, QString>::const_iterator i = data.constBegin(); i != data.constEnd(); i++)
    {
        sql += i.key() + "=";
        sql += i.value() + " ";
    }
    sql += "where ";
    for (QMap<QString, QString>::const_iterator i = where.constBegin(); i != where.constEnd(); i++)
    {
        sql += i.key() + "=";
        sql += i.value() + " ";
    }

    std::cout << "sql = " << sql.toStdString() << std::endl;

    _query->prepare(sql);
    int result = _query->exec();
	if (!result) {
		_error = _query->lastError();
		errorReport("QSqliteDataBase::updateData出错,错误代码:" + std::to_string((int)_error.type()));
	}
    return result;
}

//查找
bool QSqliteDataBase::find(QString tableName, QList<QString> key, QMap<QString, QString> where, QList<QList<QString>>* row)
{
	if (!isOpened())
		return false;
	std::lock_guard<std::mutex>locker(mutex_database);
    QString sql = "select ";
    int len = key.size();
    for (int i = 0; i < len; i++){
        sql += key.at(i);
        sql += ",";
    }
    sql.chop(1);
    sql += " from " + tableName;
    sql += " where ";
    for (QMap<QString, QString>::const_iterator i = where.constBegin(); i != where.constEnd(); i++){
        sql += i.key() + "=" + i.value() + ",";
    }
    sql.chop(1);
    _query->prepare(sql);
    if (_query->exec()){
        while (_query->next()) {
            QList<QString> j;
            for (int i = 0; i < len; i++)
            {
                j.append(_query->value(i).toString());
            }
            row->append(j);

        }
        return true;
    }
    return false;
}
//查找所有
bool QSqliteDataBase::find(QString tableName, QList<QString> key, QList<QList<QString>>* row)
{
	if (!isOpened())
		return false;
	std::lock_guard<std::mutex>locker(mutex_database);
    QString sql = "select ";
    int len = key.size();
    for (int i = 0; i < len; i++)
    {
        sql += key.at(i);
        sql += ",";
    }
    sql.chop(1);
    sql += " from " + tableName;
    //qDebug()<<sql;
    _query->prepare(sql);
    if (_query->exec())
    {
        while (_query->next())
        {
            QList<QString> j;
            for (int i = 0; i < len; i++)
            {
                j.append(_query->value(i).toString());
            }
            row->append(j);
        }
        return true;
    }
    return false;
}
QString QSqliteDataBase::getError()
{
    return _error.text();
}

//QString QSqliteDataBase::createDataTypeSqlLine( DataType dataType, ConstraintType constarin, QString argument)
//{
//    QString result;
//    result+=QString::fromStdString(SqliteDataTypeStringMap.find(dataType)->second);
//    result += " ";
//    result += QString::fromStdString(SqliteConstraintTypeStringMap.find(constarin)->second);
//    result += argument;
//    return result;
//}

void QSqliteDataBase::errorReport(std::string msg)
{
#ifdef DEVELOP_LOG_H
	DevelopLog::error(msg, DevelopLog::DevelopLogDst::HardwareRotatingFile, true);
#else
	Pi::PiStandaloneReport::errorReport("NanHangAlgorithm", msg);
#endif // DEVELOP_LOG_H
}
void QSqliteDataBase::infoReport(std::string msg)
{
#ifdef DEVELOP_LOG_H
	DevelopLog::info(msg, DevelopLog::DevelopLogDst::HardwareRotatingFile, true);
#else
	Pi::PiStandaloneReport::infoReport("NanHangAlgorithm", msg);
#endif // DEVELOP_LOG_H
}
void QSqliteDataBase::traceReport(std::string msg)
{
#ifdef DEVELOP_LOG_H
	DevelopLog::trace(msg, DevelopLog::DevelopLogDst::HardwareRotatingFile, true);
#else
	Pi::PiStandaloneReport::traceReport("NanHangAlgorithm", msg);
#endif // DEVELOP_LOG_H
}