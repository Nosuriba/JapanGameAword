#pragma once
#include <map>

class ResourceManager
{
private:
	std::map<std::string, int> _imgMap;

	ResourceManager();
	~ResourceManager();

public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&)		= delete;

	ResourceManager& operator=(const ResourceManager&)	= delete;
	ResourceManager& operator=(ResourceManager&&)		= delete;

	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	int LoadImg(std::string name);
};

