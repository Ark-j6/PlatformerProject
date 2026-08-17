#pragma once
#include <Level/GameLevel.h>
#include <string>
#include <Actor/Movable.h>

class Player;

enum class LevelState
{
	Prepare = 0,
	Start,
	DeathEffect,
	ClearEffect,
	Clear
};


class PlatformLevel : public GameLevel
{
	TYPE_DECLARATIONS(PlatformLevel, GameLevel)
		
public:
	PlatformLevel(const std::string& levelName);

private:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
	
	virtual void LoadMap() override;
	void LoadMapConfig();
	void ParsingConfig(Platformer::PlatformConfig& config, const std::string& str);

	void UpdateScreen(float deltaTime);

	void Ouch();

public:
	virtual void HandleInteraction(Platformer::Actor* target, const Platformer::Vector2& direction) override;

	void RespawnPlayer();
	void RequestNextLevel();

	void ResetLevel();

private:
	std::string levelName = "";

	LevelState levelState = LevelState::Prepare;

	// 전체 레벨 크기
	int levelWidth = 0;

	std::shared_ptr<Player> player;

	// 화면 이동 관련 기준값
	const int screenSpeed = 15;		// 화면 스크롤 속도 (플레이어 이동 속도 - 5)

	int maxScreenStartX = 0;			// 화면이 더 이상 이동할 수 없는 (레벨 끝) 영역 제한
	
	// 화면이 넘어가는 스크린 기준 위치
	const int leftBaseScreenPosX = 12;
	const int rightBaseScreenPosX = 28;

	int targetScreenX = 0;				// 자연스러운 화면 이동을 위한 변수
	float screenStartX = 0;				// 실 화면의 시작 위치

	Platformer::Vector2 currentCheckPoint = Platformer::Vector2::Zero;
	Platformer::Color lastPlayerColor = Platformer::Color::White;
};

