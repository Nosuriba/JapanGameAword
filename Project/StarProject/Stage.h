#pragma once
#include <vector>
#include <map>

struct FMF
{
	int identifier;
	int size;
	int mapWidth;
	int mapHeight;
	char chipH;
	char chipW;
	char layerCount;
	char bitCount;
};

struct StageData
{
	FMF fmf;
	std::vector<char> data;
	int readX;
};

class Stage
{
private:
	Stage();
	~Stage();

	std::map<std::string, StageData> _stages;

public:
	Stage(const Stage&) = delete;
	Stage(Stage&&) = delete;

	Stage& operator=(const Stage&) = delete;
	Stage& operator=(Stage&&) = delete;

	static Stage& GetInstance()
	{
		static Stage instance;
		return instance;
	}

	void LoadStage(std::string str);
	std::vector<char> GetStageData(std::string str);
	std::vector<char> GetStageData(std::string str, int min, int max);
};

