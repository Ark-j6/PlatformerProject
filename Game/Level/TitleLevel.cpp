#include "TitleLevel.h"
#include "TestLevel.h"
#include <Render/Renderer.h>
#include <Input/Input.h>
#include <Engine/Engine.h>
#include <cassert>

using namespace Platformer;

TitleLevel::TitleLevel()
{
	itemList.emplace_back(std::make_unique<MenuItem>(
		"Start Game"
		, []()
		{
			Engine::Get().AddNewLevel<TestLevel>("TestMap.txt");
		})
	);

	itemList.emplace_back(std::make_unique<MenuItem>(
		"Quit Game"
		, []()
		{
			// 게임 종료 호출
			Engine::Get().Quit();
		})
	);
}

void TitleLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	// 입력 처리
	// 배열의 요소 개수
	const int length = static_cast<int>(itemList.size());
	if (Input::Get().GetKeyDown(VK_UP))
	{
		// 인덱스 돌리기 (-방향)
		currentIndex = (currentIndex - 1 + length) % length;

	}
	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		// 인덱스 돌리기 (+방향)
		currentIndex = (currentIndex + 1) % length;
	}

	// 엔터 입력 처리 -> 현재 선택된 메뉴의 로직 실행
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		assert(currentIndex >= 0
			&& currentIndex < (int)itemList.size()
			&& itemList[currentIndex]->onSelected != nullptr);

		// 메뉴 아이템에 저장된 로직 실행
		itemList[currentIndex]->onSelected();
	}
}

void TitleLevel::Draw()
{
	const int screenWidth = Engine::Get().GetScreenWidth();

	const Vector2 center(Engine::Get().GetXOffset() + screenWidth / 2, Engine::Get().GetYOffset());
	
	const Vector2 title = Vector2(center.x - 6, center.y);

	// 제목 그리기
	Renderer::Get().Submit("ColorFormer", title);

	const int interval = 5;

	// 메뉴 아이템 그리기
	const int count = static_cast<int>(itemList.size());
	for (int ix = 0; ix < count; ++ix)
	{
		// 선택/미선택된 아이템 색상 처리
		Color textColor = (ix == currentIndex) ? selectdColor : unselectdColor;

		std::string& str = itemList[ix]->text;

		// 아이템 그리기
		Renderer::Get().Submit(
			str,
			Vector2(screenWidth - interval - str.length(), center.y + interval + ix),
			textColor
		);
	}
}
