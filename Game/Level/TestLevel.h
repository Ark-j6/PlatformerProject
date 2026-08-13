#pragma once
#include <Level/Level.h>
#include <string>

class Player;

enum class LevelState
{
	Prepare = 0,
	Start,
	ClearEffect,
	Clear
};


class TestLevel : public Platformer::Level
{
	TYPE_DECLARATIONS(TestLevel, Level)
		
public:
	TestLevel(const std::string& levelName);

private:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
	
	void LoadMap();

	void UpdateScreen(float deltaTime);

	void VictoryEffect(float deltaTime);

public:
	Platformer::Actor* GetActorAt(const Platformer::Vector2& nextPosition);
	bool CanMove(const Platformer::Actor* other, Platformer::Color color);
	void HandleInteraction(Platformer::Actor* target, const Platformer::Vector2& direction);

	inline bool CheckValidXPos(const int posX) const { return !(posX < 0 || screenStartX < 0); }
	inline bool CheckValidYPos(const int posY) const { return !(posY < 0 || posY > levelHeight); }
	void ChangeActorColors();

private:
	const std::string victory = "Stage Clear!";
	const std::string fail = "GAME OVER";

	std::string levelName = "";

	LevelState levelState = LevelState::Prepare;

	// 전체 레벨 크기
	int levelWidth = 0;
	int levelHeight = 0;

	int screenWidth = 0;

	std::shared_ptr<Player> player;

	// 화면 이동 관련 기준값
	const int screenSpeed = 15;		// 화면 스크롤 속도 (플레이어 이동 속도 - 5)

	int maxScreenStartX = 0;			// 화면이 더 이상 이동할 수 없는 (레벨 끝) 영역 제한
	
	// 화면이 넘어가는 스크린 기준 위치
	const int leftBaseScreenPosX = 12;
	const int rightBaseScreenPosX = 28;

	int targetScreenX = 0;				// 자연스러운 화면 이동을 위한 변수
	float screenStartX = 0;				// 실 화면의 시작 위치

	float victoryEffectTimer = 0.0f;
};

