#pragma once
#include <vector>
#include <map>
#include "Processing/Geometry.h"

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

struct ChipInfo
{
	char no;
	int x, y;
};

struct StageData
{
	FMF fmf;
	std::vector<ChipInfo> data;
	int readX;
};

class Stage
{
private:
	Stage();
	~Stage();

	int count;
	int fmf_h;
	std::map<std::string, StageData> _stages;
	std::string name;

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
	const bool LoadCheck();

	const Vector2 GetStageSize();
	std::vector<ChipInfo> GetStageData();
	std::vector<ChipInfo> GetStageData(int min, int max);
};

