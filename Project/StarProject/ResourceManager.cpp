#include "ResourceManager.h"

#include <DxLib.h>

ResourceManager::ResourceManager()
{
	_imgMap.clear();
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

int ResourceManager::LoadSound(std::string name)
{
	if (_soundMap.find(name) == _soundMap.end())
	{
		SetUseASyncLoadFlag(true);
		auto str = "../Sound/" + name;
		_soundMap[name] = LoadSoundMem(str.c_str());
		SetUseASyncLoadFlag(false);
	}
	return _soundMap[name];
}
