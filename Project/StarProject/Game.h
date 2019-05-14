#pragma once

#include <memory>
#include "Scene/Scene.h"
#include "Processing/Geometry.h"
#include <vector>

class Game
{
public:
	static Game & GetInstance()
	{
		return *s_Instance;
	}

	void Init();
	void Run();
	void End();

	void ChangeScene(Scene *scene);

	const Vector2 GetScreenSize() const;
	const std::vector<int> GetShaderHandle() const;

	int wavehandle;
	int shadowhandle;

	std::vector<int> _handle;

private:
	Game();
	~Game();
	struct GameDeleter
	{
		void operator()(Game * game) const
		{
			delete game;
		}
	};

	static std::unique_ptr<Game, GameDeleter> s_Instance;
	std::shared_ptr<Scene> _scene;
};

