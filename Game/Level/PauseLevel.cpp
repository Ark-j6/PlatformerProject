#include "PauseLevel.h"
#include <Input/Input.h>
#include <Render/Renderer.h>
#include <Engine/Engine.h>
#include <Game/GameManager.h>
#include <fstream>
#include <cassert>

using namespace Platformer;

PauseLevel::PauseLevel()
{
	LoadPauseMenu();

	// 메뉴 아이템 생성
	itemList.emplace_back(std::make_unique<MenuItem>("Resume Game"
		, []()
	{
		// 메뉴 토글 함수 호출
		GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
		game.TogglePauseMenu();
	})
	);
	itemList.emplace_back(std::make_unique<MenuItem>("Back To Title"
		, []()
	{
		// 타이틀 메뉴 호출
		GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
		game.BackToTitle();
	})
	);
	itemList.emplace_back(std::make_unique<MenuItem>("Quit Game"
		, []()
	{
		// 타이틀 메뉴 호출
		Engine::Get().Quit();
	})
	);
}

void PauseLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	// 입력 처리
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
		game.TogglePauseMenu();

		// 인덱스 초기화
		currentIndex = 0;
	}

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

void PauseLevel::Draw()
{
	for (int i = 0; i < menuString.size(); ++i)
	{
		Renderer::Get().Submit(menuString[i], Vector2(0, i));
	}

	// 메뉴 아이템 그리기
	const int count = static_cast<int>(itemList.size());
	for (int ix = 0; ix < count; ++ix)
	{
		// 선택/미선택된 아이템 색상 처리
		Color textColor = (ix == currentIndex) ? selectdColor : unselectdColor;

		// 아이템 그리기
		Renderer::Get().Submit((ix == currentIndex) ? "> " + itemList[ix]->text : itemList[ix]->text
			, Vector2((ix == currentIndex) ? 21 : 23, 10 + ix)
			, textColor
		);
	}
}

void PauseLevel::LoadPauseMenu()
{
	// 최종 경로 조립
	std::string path = std::string("../Assets/Pause.txt");

	/* C++ 스타일 */
	// 파일 열기 - 파일 크기와 실제 읽을 바이트 수를 일치시키기 위해 binary 사용
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
	{
		assert(false && "falied to open stage file\n");
		return;
	}

	std::string str;
	while (std::getline(file, str))
	{
		if (!str.empty() && str.back() == '\r')
		{
			str.pop_back();
		}

		menuString.emplace_back(str);
	}
}
