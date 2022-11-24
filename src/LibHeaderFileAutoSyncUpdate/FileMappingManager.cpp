#include "FileMappingManager.h"
FileMappingManager& FileMappingManager::Instance() {
	static std::unique_ptr<FileMappingManager>instance_ptr =
		std::unique_ptr<FileMappingManager>(new FileMappingManager);
	return *instance_ptr;
}
FileMappingManager::~FileMappingManager() {

}
FileMappingManager::FileMappingManager() {

}
