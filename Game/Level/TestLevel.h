#pragma once
#include <Level/Level.h>
#include <string>
#include <vector>

class TestLevel : public Platformer::Level
{
	TYPE_DECLARATIONS(TestLevel, Level)

private:
	virtual void OnInitialized() override;
	virtual void Draw() override;
	
	void LoadMap(const std::string& fileName);

	std::vector<char> levelData;

	int levelWidth = 0;
	int levelHeight = 0;

public:
	bool CanMove(const Platformer::Vector2& nextPosition);
};

