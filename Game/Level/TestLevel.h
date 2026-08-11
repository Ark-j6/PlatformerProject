#pragma once
#include <Level/Level.h>
#include <string>
#include <vector>

class TestLevel : public Platformer::Level
{
	TYPE_DECLARATIONS(TestLevel, Level)

private:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
	
	void LoadMap(const std::string& fileName);

public:
	bool CanMove(const Platformer::Vector2& nextPosition, Platformer::Color color);

	void CheckPlayerXPos(int playerX);
	void ChangeActorColors();

private:
	// 전체 레벨 크기
	int levelWidth = 0;
	int levelHeight = 0;

	std::vector<char> levelData;

	// 화면 이동 관련 기준값
	bool isScreenMoving = false;
	const int wannaOffsetX = 5;		// 화면상 플레이어 희망 위치
	const int screenSpeed = 15;			// 화면 스크롤 속도 (플레이어 이동 속도 - 5)

	int maxScreenStartX = 0;			// 화면이 더 이상 이동할 수 없는 (레벨 끝) 영역 제한
	int leftBasePosX = 16;				// 화면이 넘어가는 절대위치 기준
	int rightBasePosX = 24;			// 화면이 넘어가는 절대위치 기준

	int targetScreenX = 0;				// 자연스러운 화면 이동을 위한 변수
	float screenStartX = 0;				// 실 화면의 시작 위치

	// 스크린 크기
	const int screenWidth = 40;
};

