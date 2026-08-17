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
	menuList[static_cast<int>(MenuState::InGame)].reset();

	menuIndex = MenuState::Title;
	mainLevel = menuList[static_cast<int>(menuIndex)];
}

void GameManager::StartFirstLevel()
{
	totalPlayTime = 0;
	currentStageIndex = 0;

	menuIndex = MenuState::InGame;
	menuList.emplace_back(std::make_shared<PlatformLevel>(stageList[currentStageIndex]));
	mainLevel = menuList[static_cast<int>(menuIndex)];
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
	menuList.emplace_back(std::make_shared<ResultLevel>(totalPlayTime));
	menuIndex = MenuState::Result;
	mainLevel = menuList[static_cast<int>(menuIndex)];
}

void GameManager::CloseReulstLevel()
{
	menuList[static_cast<int>(MenuState::Result)].reset();

	menuIndex = MenuState::Title;
	mainLevel = menuList[static_cast<int>(menuIndex)];
}
