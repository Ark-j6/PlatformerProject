#include "TitleLevel.h"
#include <Level/PlatformLevel.h>
#include <Input/Input.h>
#include <Render/Renderer.h>
#include <Actor/Player.h>
#include <Actor/Platform.h>
#include <Actor/Terrain.h>
#include <Game/GameManager.h>
#include <fstream>
#include <cassert>

using namespace Platformer;

TitleLevel::TitleLevel()
{}

void TitleLevel::OnInitialized()
{
	Level::OnInitialized();

	itemList.emplace_back(std::make_unique<MenuItem>(
		"Start Game"
		, []()
	{
		GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
		game.StartFirstLevel();
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

	screenWidth = Engine::Get().GetScreenWidth();
	LoadMap();
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
	Level::Draw();

	const int screenWidth = Engine::Get().GetScreenWidth();
	const Vector2 center(Engine::Get().GetXOffset() + screenWidth / 2, Engine::Get().GetYOffset());
	size_t length = title.length();
	const Vector2 titlePos = Vector2(center.x - static_cast<int>(length) / 2, center.y);
	//// 제목 그리기
	Renderer::Get().Submit(title, titlePos);

	const int interval = 7;

	// 메뉴 아이템 그리기
	const int count = static_cast<int>(itemList.size());
	for (int ix = 0; ix < count; ++ix)
	{
		// 선택/미선택된 아이템 색상 처리
		Color textColor = (ix == currentIndex) ? selectdColor : unselectdColor;
		std::string str = (ix == currentIndex) ? ">   " + itemList[ix]->text : itemList[ix]->text;

		// 아이템 그리기
		Renderer::Get().Submit(str, Vector2((ix == currentIndex) ? 30 : 34, interval + ix), textColor);
	}

	Renderer::Get().Submit(control, Vector2(26, 23));
	Renderer::Get().Submit(input, Vector2(26, 26));
}

void TitleLevel::LoadMap()
{
	// 최종 경로 조립
	std::string path = std::string("../Assets/Title.txt");

	/* C++ 스타일 */
	// 파일 열기 - 파일 크기와 실제 읽을 바이트 수를 일치시키기 위해 binary 사용
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
	{
		assert(false && "falied to open stage file\n");
		return;
	}

	Vector2 position = Vector2::Zero;
	bool isActorSpawning = false;
	Platform* current = nullptr;
	unsigned short size = 0;

	bool isPlayerFound = false;
	Vector2 playerPos = Vector2::Zero;

	std::string str;
	while (std::getline(file, str))
	{
		position.x = 0;

		for (int n = 0; n < str.length(); ++n)
		{
			if (str[n] == '\n' || str[n] == '\r')
			{
				position.x = 0;
				break;
			}

			switch (str[n])
			{
			case 'P':
				isPlayerFound = true;
				playerPos = position;
				break;
			case '#':
			case '=':
			{
				std::string s = std::string(1, str[n]);
				SpawnActor<Terrain>(s, position);
				break;
			}
			case '(':
			case '{':
			case '[':
			{
				size = 1;
				break;
			}
			case ')':
			case '}':
			case ']':
				assert(current && "* finished, but no current platform");
				current->ChangeWidth(size);
				current->isColorChangable = true;
				current = nullptr;
				size = 0;
				isActorSpawning = false;
				break;
			case 'R':
				if (!isActorSpawning && size == 1)
				{
					isActorSpawning = true;
					current = SpawnActor<Platform>(position, Color::Red).get();
				}
				else
				{
					++size;
				}
				break;
			case 'G':
				if (!isActorSpawning && size == 1)
				{
					isActorSpawning = true;
					current = SpawnActor<Platform>(position, Color::Green).get();
				}
				else
				{
					++size;
				}
				break;
			case 'B':
				if (!isActorSpawning && size == 1)
				{
					isActorSpawning = true;
					current = SpawnActor<Platform>(position, Color::Blue).get();
				}
				else
				{
					++size;
				}
				break;
			case 'W':
				if (!isActorSpawning && size == 1)
				{
					isActorSpawning = true;
					current = SpawnActor<Platform>(position, Color::White).get();
				}
				else
				{
					++size;
				}
				break;
			default:
				break;
			}

			++position.x;
		}

		++position.y;
	}

	// 플레이어의 이동 처리를 가장 후순으로 만들기 위해서 의도적으로 마지막에 Spawn시도
	if (isPlayerFound)
	{
		player = SpawnActor<Player>(playerPos);
	}

	levelHeight = position.y;


	// 파일 닫기
	file.close();
}