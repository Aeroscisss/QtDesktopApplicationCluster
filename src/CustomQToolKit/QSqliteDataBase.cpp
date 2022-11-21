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

//���캯��������Ϊ�������ƣ����������û���������
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
//�����������ر����ݿ�
QSqliteDataBase::~QSqliteDataBase()
{
    closeDataBase();
}
QSqlDatabase* QSqliteDataBase::getQSqlDatabase()
{
    return &_database;
}
//�����ݿ⣺������Ĭ�ϴ��ڹ�QSqlDataBase
bool QSqliteDataBase::openDataBase()
{
    std::lock_guard<std::mutex>locker(mutex_database);
    if (!_database.open()){
        _error = _database.lastError();
        DevelopLog::error("fail open sqlite database " + _database.connectionName().toStdString()+" error:"+_error.databaseText().toStdString(), DevelopLog::DevelopLogDst::GeneralDailyFile, true);
        QMessageBox::warning(nullptr, QObject::tr("Warning"), _database.lastError().text());//�޷������ݿ⣬�������淢�ʹ�����Ϣ
        return false;//��ʧ��
    }
    else
        _databaseOpened = true;
    return true;//�򿪳ɹ�
}
//��ȡ���ݿ��Ƿ��Ѿ���
bool QSqliteDataBase::isOpened()
{
    return _databaseOpened;
}

//�ر����ݿ⣬�˺���Ĭ�Ϲرձ����ڵ��ڹ�QSqlDataBase
void  QSqliteDataBase::closeDataBase()
{
    std::lock_guard<std::mutex>locker(mutex_database);
    _database.close();
    _databaseOpened = false;
}
//���������֣���database���ұ���Ƿ���ڣ�����boolֵ
bool QSqliteDataBase::checkTableExist(QString tableName)
{
    QStringList a=_database.tables();
    if (!isOpened())
        return false;
    std::lock_guard<std::mutex>locker(mutex_database);
    QString sql;
    sql = "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name ='" + tableName + "'";//�Ƿ��Ѿ����ڱ�car_bayonet_info
    _query->prepare(sql);
    sql += ";";
    if (!_query->exec())
        return false;
    return true;
}

//ɾ����񣬴˺������ݸ����ı�����Ƶ�QStringɾ������Ӧ�ı��
bool QSqliteDataBase::deleteTable(QString tableName)//ɾ�����finished��
{
	if (!isOpened())
		return false; 
    std::lock_guard<std::mutex>locker(mutex_database);
    //�������ڣ������ھ�д�������Ϣ������
    if (checkTableExist(tableName)){
        QString error = QString::fromUtf8("ɾ�����") + tableName + QString::fromUtf8("ʧ�ܣ���񲻴��ڻ�����");
        _error = error;
        return false;//ִ��ʧ��
    }
    //�����ڣ����Խ��б��ɾ������
    QString sql;
    sql = "DROP TABLE " + tableName;
    sql += ";";
    _query->prepare(sql);
    if (_query->exec())
        return true;//ִ�гɹ�
    else{
        QString error = QString::fromUtf8("ɾ�����") + tableName + QString::fromUtf8("ʧ�ܣ�ԭ��δ֪");
        _error = error;
        _error = _database.lastError();
        return false;//ִ��ʧ��
    }
}

//��finished��������񣬸ú������ڹ���QSqlDataBase�����һ�����Qmap���tabeldata�ڵ�pair�ĸ�ʽΪ����-��������
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
		errorReport("QSqliteDataBase::createTable����,�������:" + std::to_string((int)_error.type()));
	}
    return  result;
}


//��finished����ָ����table����������ݣ������������
bool  QSqliteDataBase::addData(QString tableName, QMap<QString, QString> tableData)//��������
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
        errorReport("QSqliteDataBase::addData����,�������:"+std::to_string((int)_error.type()));
    }
    return true;
}


//��finished����ָ����table����ɾ�����ݣ�����������ֺͼ�¼
bool QSqliteDataBase::deleteData(QString tableName, QMap<QString, QString> where)//ɾ��һ����¼
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
		errorReport("QSqliteDataBase::deleteData����,�������:" + std::to_string((int)_error.type()));
	}
    return result;
}

//�������ݿ����
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
		errorReport("QSqliteDataBase::sendDatabaseQuery����,�������:" + std::to_string((int)_error.type()));
	}
    return result;
}

//��������
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
		errorReport("QSqliteDataBase::updateData����,�������:" + std::to_string((int)_error.type()));
	}
    return result;
}

//����
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
//��������
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