#include <QObject>
#include<memory>
class FileMappingManager:public QObject
{
	Q_OBJECT
public:
	static FileMappingManager& Instance();
	~FileMappingManager();
protected:
private:
	FileMappingManager();
};