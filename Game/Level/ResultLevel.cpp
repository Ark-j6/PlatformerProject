#include "ResultLevel.h"
#include <Game/GameManager.h>
#include <Render/Renderer.h>
#include <Input/Input.h>
#include <fstream>
#include <cassert>

using namespace Platformer;

ResultLevel::ResultLevel(float resultTime)
{
	LoadPauseMenu();

	int intTime = static_cast<int>(resultTime);

	minutes = intTime / 60;
	seconds = intTime % 60;
	milliSeconds = static_cast<int>((resultTime - intTime) * 1000.f);

	result = std::to_string(minutes) + ":" + std::to_string(seconds) + "." + std::to_string(milliSeconds);
}

void ResultLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);


	if (Input::Get().GetKeyDown(VK_RETURN) || Input::Get().GetKeyDown(VK_ESCAPE))
	{
		GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
		game.BackToTitle();
	}
}

void ResultLevel::Draw()
{
	for (int i = 0; i < menuString.size(); ++i)
	{
		Renderer::Get().Submit(menuString[i], Vector2(0, i));
	}

	int length = static_cast<int>((menuString[0].length() / 2) - (result.length() / 2));

	Renderer::Get().Submit(result, Vector2(length, 14), Color::Yellow);
	Renderer::Get().Submit(enter, Vector2(30, 19), Color::Yellow);
}

void ResultLevel::LoadPauseMenu()
{
	// 최종 경로 조립
	std::string path = std::string("../Assets/Result.txt");

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
