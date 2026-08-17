#pragma once
#include <Engine\Engine.h>
#include <string>
#include <vector>

class Platformer::Level;

enum class MenuState
{
	Title = 0,
	Pause,
	InGame,
	Result
};

class GameManager : public Platformer::Engine
{
public:
	GameManager();
	~GameManager() = default;

private:
	virtual void Tick(float deltaTime) override;

	void LoadStageList();

public:
	void TogglePauseMenu();
	void BackToTitle();
	void StartFirstLevel();
	void LoadNextGameLevel();
	void ShowResultLevel();
	void CloseReulstLevel();

private:
	MenuState menuIndex = MenuState::Title;
	std::vector<std::shared_ptr<Platformer::Level>> menuList;
	std::vector<std::string> stageList;
	int currentStageIndex = 0;

	float totalPlayTime = 0;
};

