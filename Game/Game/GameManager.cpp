#include "GameManager.h"
#include <Level/TitleLevel.h>
#include <Level/PlatformLevel.h>

#include <fileSystem>

GameManager::GameManager()
{
	LoadStageList();

	menuList.emplace_back(std::make_shared<TitleLevel>());
	menuIndex = MenuState::Title;
	mainLevel = menuList[static_cast<int>(menuIndex)];
}

void GameManager::LoadStageList()
{
	const std::string stagePath = "../Assets/Stages/";

	for (const auto& entry : std::filesystem::directory_iterator(stagePath))
	{
		if (!entry.is_regular_file())
			continue;

		if (entry.path().extension() != ".txt")
			continue;
		
		stageList.emplace_back(entry.path().filename().string());
	}

	currentStageIndex = 0;
}

void GameManager::TogglePauseMenu()
{
	if (menuIndex != MenuState::Title)
	{
		if (menuIndex == MenuState::InGame)
			menuIndex = MenuState::Pause;
		else
			menuIndex = MenuState::InGame;

		mainLevel = menuList[static_cast<int>(menuIndex)];
	}
}

void GameManager::LoadNextGameLevel()
{
	++currentStageIndex;
	if (currentStageIndex >= stageList.size())
	{
		menuIndex = MenuState::Title;
		mainLevel = menuList[static_cast<int>(menuIndex)];
		return;
	}

	AddNewLevel<PlatformLevel>(stageList[currentStageIndex]);
}
