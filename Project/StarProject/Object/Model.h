#pragma once
#include <map>
#include <string>
#include <vector>

class Model
{
private:
	Model();

	std::map<std::string, int> _handle;

	std::vector<int> _chara;
	
public:
	static Model& Instance() {
		static Model instance;
		return instance;
	}

	int Load(const char* handle);

	const std::vector<int> GetChara() const;

	~Model();
};

