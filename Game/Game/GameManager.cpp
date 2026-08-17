#include "GameManager.h"
#include <Level/TitleLevel.h>
#include <Level/PauseLevel.h>
#include <Level/PlatformLevel.h>
#include <Level/ResultLevel.h>

#include <fileSystem>

GameManager::GameManager()
{
	LoadStageList();
	currentStageIndex = 0;

	menuList.emplace_back(std::make_shared<TitleLevel>());
	menuList.emplace_back(std::make_shared<PauseLevel>());
	menuList.emplace_back(std::make_shared<PlatformLevel>(stageList[currentStageIndex]));
	menuList.emplace_back(std::make_shared<ResultLevel>());

	menuIndex = MenuState::Title;
	mainLevel = menuList[static_cast<int>(menuIndex)];
}

void GameManager::Tick(float deltaTime)
{
	Engine::Tick(deltaTime);

	if (menuIndex == MenuState::InGame)
	{
		totalPlayTime += deltaTime;
	}
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

void GameManager::BackToTitle()
{
	menuIndex = MenuState::Title;
	mainLevel = menuList[static_cast<int>(menuIndex)];
}

void GameManager::StartFirstLevel()
{
	totalPlayTime = 0;
	currentStageIndex = 0;

	menuIndex = MenuState::InGame;
	AddNewLevel<PlatformLevel>(stageList[currentStageIndex]);
	menuList[static_cast<int>(menuIndex)] = nextLevel;
}

void GameManager::LoadNextGameLevel()
{
	++currentStageIndex;
	if (currentStageIndex >= stageList.size())
	{
		ShowResultLevel();
		return;
	}

	AddNewLevel<PlatformLevel>(stageList[currentStageIndex]);
	menuList[static_cast<int>(MenuState::InGame)] = nextLevel;
}

void GameManager::ShowResultLevel()
{
	menuIndex = MenuState::Result;

	if (auto result = std::dynamic_pointer_cast<ResultLevel>(menuList[static_cast<int>(menuIndex)]))
	{
		result->UpdateResult(totalPlayTime);
		mainLevel = menuList[static_cast<int>(menuIndex)];
	}
}

void GameManager::CloseReulstLevel()
{
	menuIndex = MenuState::Title;
	mainLevel = menuList[static_cast<int>(menuIndex)];
}
