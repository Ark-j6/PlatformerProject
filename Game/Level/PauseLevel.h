#pragma once
#include <Level/LevelItem.h>
#include <Level/GameLevel.h>

class PauseLevel : public Platformer::Level
{
public:
	PauseLevel();

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	void LoadPauseMenu();

private:
	std::vector<std::string> menuString;


	// 현재 활성화된 메뉴 아이템 인덱스
	int currentIndex = 0;

	// 선택된 메뉴 아이템의 색상
	Platformer::Color selectdColor = Platformer::Color::Yellow;

	// 미선택된 메뉴 아이템의 색상
	Platformer::Color unselectdColor = Platformer::Color::White;

	std::vector<std::unique_ptr<MenuItem>> itemList;

};

