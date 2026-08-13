#pragma once
#include <Engine\Engine.h>
#include <string>
#include <vector>

class Platformer::Level;

enum class MenuState
{
	Title = 0,
	InGame,
	Pause
};

class GameManager : public Platformer::Engine
{
public:
	GameManager();
	~GameManager() = default;

private:
	void LoadStageList();

public:
	void TogglePauseMenu();

	void LoadNextGameLevel();

private:
	MenuState menuIndex = MenuState::Title;
	std::vector<std::shared_ptr<Platformer::Level>> menuList;
	std::vector<std::string> stageList;
	int currentStageIndex = 0;
};

