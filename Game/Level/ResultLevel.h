#pragma once
#include <Level\Level.h>
#include <string>

class ResultLevel : public Platformer::Level
{
	TYPE_DECLARATIONS(ResultLevel, Level)

public:
	ResultLevel(float resultTime);

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	void LoadPauseMenu();

private:
	int minutes = 0;
	int seconds = 0;
	int milliSeconds = 0;

	std::vector<std::string> menuString;
	std::string result = "";
	const std::string enter = "PRESS ENTER TO TITLE";
};

