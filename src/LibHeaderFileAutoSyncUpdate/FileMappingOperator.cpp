#include "FileMappingOperator.h"
#include <memory>
#include <QCryptographicHash>
#include <QTextStream>
#include <QDir>
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
	//m_thread = std::make_unique< std::thread>();
	ruleQueue = std::make_unique<MultiThreadQueue<FileMappingRule>>();
}

FileMappingOperator::~FileMappingOperator()
{
	threadIsInterrupted = true;
	if(m_thread.joinable())
		m_thread.join();
}

void FileMappingOperator::threadLoopRun()
{
	threadIsInterrupted = false;
	m_thread = std::thread(&ruleOpLoop);
}

void FileMappingOperator::ruleOpLoop()
{
	while (!FileMappingOperator::Instance().threadIsInterrupted)
	{
		GlobalMessageRepost::Instance().sendNewMsg("new loop",1);
		FileMappingRule rule;
		FileMappingOperator::Instance().ruleQueue->wait(rule);
		FileMappingOperator::Instance().applyRule(rule);
	}
}

void FileMappingOperator::applyRule(FileMappingRule rule)
{
	QStringList filters = rule.filterList();//获取正则匹配列表
	QStringList srcs = rule.srcList();//获取源值列表
	QMap<QString, CustomFileInfo>srcFileMap;//源文件Map
	convertPathValueIntoFileInfo(srcs, srcFileMap);
	regexCheck(srcFileMap, filters);
	QString dst = rule.dst();//获取目的地值列表
	QMap<QString, CustomFileInfo>dstFileMap;//目的地文件Map
	if (QFileInfo(dst).isDir()) {
		findAllFilesUnderFolder(dst, dstFileMap);
		if (!dst.isEmpty()) {
			QChar lastChar = *(dst.end() - 1);
			if (lastChar != '/' && lastChar != '\\')
				dst += '/';
		}
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
		else {//根本不存在
			copyFile(srcFileInfo.filePath, dst+srcFileInfo.fileCompleteName, true);
		}
	}
}

void FileMappingOperator::convertPathValueIntoFileInfo(QStringList files, QMap<QString, CustomFileInfo>& fileInfoMap)
{
	QMap<QString, CustomFileInfo> newMap;
	QFileInfo fileInfo;
	for (auto value : files) {
		fileInfo= QFileInfo(value);
		if (!fileInfo.exists()) {//不存在，跳过
			continue;
		}
		if (fileInfo.isFile()) {
			CustomFileInfo customFileInfo=CustomFileInfo(value);
			customFileInfo.isFileFlag = true;
			customFileInfo.fileCompleteName = fileInfo.completeBaseName() + "." + fileInfo.suffix();
			customFileInfo.fileBaseName = fileInfo.completeBaseName();
			customFileInfo.fileByteSize = fileInfo.size();
			newMap.insert(customFileInfo.fileCompleteName, customFileInfo);
			continue;
		}
		if (fileInfo.isDir()) {
			findAllFilesUnderFolder(value, fileInfoMap);
			continue;
		}
	}
}

void FileMappingOperator::findAllFilesUnderFolder(QString folderPath, QMap<QString, CustomFileInfo>& fileInfoMap)
{
		QDir dir(folderPath);
	QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	for (auto fileInfo:file_list) {
		CustomFileInfo customFileInfo = CustomFileInfo(fileInfo.filePath());
		customFileInfo.isFileFlag = true;
		customFileInfo.fileCompleteName = fileInfo.completeBaseName()+"."+ fileInfo.suffix();
		customFileInfo.fileBaseName = fileInfo.completeBaseName();
		customFileInfo.fileByteSize = fileInfo.size();
		fileInfoMap.insert(customFileInfo.filePath, customFileInfo);
		continue;
	}
	QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (int i = 0; i != folder_list.size(); i++)
	{
		QString folderName = folder_list.at(i).absoluteFilePath();
		findAllFilesUnderFolder(folderName,fileInfoMap);
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

void FileMappingOperator::regexCheck(QMap<QString, CustomFileInfo>& map, QStringList regexList)
{
	QMap<QString, CustomFileInfo> newMap;
	for (auto iter = map.begin(); iter != map.end();iter++) {
		QRegExp regExp;
		for (auto reg : regexList) {
			regExp.setPattern(reg);
			if(!reg.isValidUtf16())
				continue;
			if (regExp.indexIn(iter.key()) >= 0) {//匹配ok
				newMap.insert(iter.key(),iter.value());
			}
		}
	}
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


