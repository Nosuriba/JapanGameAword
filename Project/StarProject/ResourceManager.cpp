#include "ResourceManager.h"

#include <DxLib.h>

ResourceManager::ResourceManager()
{
	_imgMap.clear();
	//LoadImg("../img/umi.png");
}


ResourceManager::~ResourceManager()
{
	for (auto& img : _imgMap)
	{
		DeleteGraph(img.second);
	}
}

int ResourceManager::LoadImg(std::string name)
{
	if (_imgMap.find(name) == _imgMap.end())
	{
		_imgMap[name] = LoadGraph(name.c_str());
	}
	return _imgMap[name];
}
