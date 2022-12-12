#include "FileMappingOperator.h"
#include <memory>
#include <QCryptographicHash>
#include <QTextStream>
#include "QFileInfo"
#include "GlobalMessageRepost.h"
FileMappingOperator& FileMappingOperator::Instance()
{
	static std::unique_ptr<FileMappingOperator>instance_ptr=std::unique_ptr<FileMappingOperator>(new FileMappingOperator);
	return *instance_ptr;
}

FileMappingOperator::FileMappingOperator()
{
	threadIsInterrupted = false;
	m_thread = std::make_unique< std::thread>();
	ruleQueue = std::make_unique<MultiThreadQueue<FileMappingRule>>();
}

FileMappingOperator::~FileMappingOperator()
{
	m_thread->join();
}

void FileMappingOperator::threadLoopRun()
{
	while (!FileMappingOperator::Instance().threadIsInterrupted)
	{
		FileMappingRule rule;
		ruleQueue->wait(rule);
	}
}

void FileMappingOperator::applyRule(FileMappingRule rule)
{
	struct CustomFileInfo {
		explicit CustomFileInfo(QString path) 
			:filePath(path){}
		bool isFile = false;
		QString filePath;
		QString fileCompleteName;
		QString fileBaseName;
		QString md5;
		quint64 fileByteSize;
	};
	QStringList filters = rule.filterList();//正则匹配列表
	QFileInfo fileInfo;//文件Info对象
	 
	QStringList srcs = rule.srcList();
	QMap<QString,CustomFileInfo>srcFileMap;
	for (auto srcPath : srcs) {
		regexCheck(srcs, filters);
		CustomFileInfo customInfoStruct(srcPath);
		fileInfo = QFileInfo(srcPath);
		if(!fileInfo.isFile())
			continue;
		customInfoStruct.isFile = true;
		customInfoStruct.fileCompleteName= fileInfo.completeBaseName();
		customInfoStruct.fileCompleteName = fileInfo.baseName();
		customInfoStruct.fileByteSize = fileInfo.size();
		srcFileMap.insert(customInfoStruct.fileCompleteName,customInfoStruct);
	}
	QStringList dsts= rule.dstList();
	QMap<QString, CustomFileInfo>dstFileMap;
	for (auto dstPath : dsts) {
		regexCheck(dsts, filters);
		CustomFileInfo customInfoStruct(dstPath);
		fileInfo = QFileInfo(dstPath);
		if (!fileInfo.isFile())
			continue;
		customInfoStruct.isFile = true;
		customInfoStruct.fileCompleteName = fileInfo.completeBaseName();
		customInfoStruct.fileCompleteName = fileInfo.baseName();
		customInfoStruct.fileByteSize = fileInfo.size();
		dstFileMap.insert(customInfoStruct.fileCompleteName, customInfoStruct);
	}
	for (auto srcFileInfo : srcFileMap) {
		auto iter = dstFileMap.find(srcFileInfo.fileCompleteName);
		if (iter != dstFileMap.end()) {//确实存在
			CustomFileInfo dstFileInfo = iter.value();
			if (srcFileInfo.fileByteSize < 1024 * 1024 * 10) {//小于10MB
				srcFileInfo.md5 = smallFileMd5(srcFileInfo.filePath);
				dstFileInfo.md5 = smallFileMd5(dstFileInfo.filePath);
			}
			else {
				srcFileInfo.md5 = bigFileMd5(srcFileInfo.filePath);
				dstFileInfo.md5 = bigFileMd5(dstFileInfo.filePath);
			}
			if (srcFileInfo.md5 != dstFileInfo.md5) {//文件有改变
				//移动src到dst
				copyFile(srcFileInfo.filePath, dstFileInfo.filePath,true);
			}
		}
	}
}

QString FileMappingOperator::smallFileMd5(QString path)
{
	QFile file(path);
	QTextStream in(&file);
	QString str;
	QByteArray bb;
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		str = in.readAll();
		//qDebug() << str;
		bb = QCryptographicHash::hash(str.toLatin1(), QCryptographicHash::Md5);
		str = bb.toHex();
	}
	file.close();
	return  str;
}

QString FileMappingOperator::bigFileMd5(QString path)
{
	QString str;
	QFile localFile(path);

	if (!localFile.open(QIODevice::ReadOnly))
	{
		//qDebug() << "file open error.";
		return QString();
	}

	QCryptographicHash ch(QCryptographicHash::Md5);

	quint64 totalBytes = 0;
	quint64 bytesWritten = 0;
	quint64 bytesToWrite = 0;
	quint64 loadSize = 1024 * 4;
	QByteArray buf;

	totalBytes = localFile.size();
	bytesToWrite = totalBytes;

	while (1){
		if (bytesToWrite > 0){
			buf = localFile.read(qMin(bytesToWrite, loadSize));
			ch.addData(buf);
			bytesWritten += buf.length();
			bytesToWrite -= buf.length();
			buf.resize(0);
		}
		else{
			break;
		}
		if (bytesWritten == totalBytes){
			break;
		}
	}
	localFile.close();
	QByteArray md5 = ch.result();
	str = md5.toHex();
	//qDebug() << md5.toHex();
	return  str;
}

void FileMappingOperator::regexCheck(QStringList& list, QStringList regexList)
{
	QStringList newList;
	for (auto str : list) {
		bool matchSuccess = false;
		QRegExp regExp;
		for (auto reg : regexList) {
			regExp.setPattern(reg);
			if(!reg.isValidUtf16())
				continue;
			if (regExp.indexIn(str) > 0) {
				matchSuccess = true;
				break;//匹配成功
			}
		}
		if (matchSuccess) {
			newList.append(str);
		}
	}
	list=newList;
}

bool FileMappingOperator::copyFile(QString srcPath, QString dstPath, bool coverFileIfExist)
{
	srcPath.replace("\\", "/");
	dstPath.replace("\\", "/");
	if (srcPath == dstPath) {
		return true;
	}
	if (!QFile::exists(srcPath)) {  //源文件不存在
		QString errMsg = "File[" + srcPath + "] does not exist";
		GlobalMessageRepost::Instance().sendNewMsg(errMsg);
		return false;
	}
	if (QFile::exists(dstPath)) {
		if (coverFileIfExist) {
			QFile::remove(dstPath);
		}
	}
	if (!QFile::copy(srcPath, dstPath)) {
		QString errMsg = "File[" + srcPath + "] copy fail";
		GlobalMessageRepost::Instance().sendNewMsg(errMsg);
		return false;
	}
	QString msg = "File[" + srcPath + "] copy success";
	GlobalMessageRepost::Instance().sendNewMsg(msg);
	return true;
}


