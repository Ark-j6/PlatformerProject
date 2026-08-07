#include "TestLevel.h"
#include <Math/Vector2.h>
#include <Actor/Player.h>
#include <Actor/Wall.h>
#include <Actor/Platform.h>

#include <fstream>
#include <cassert>

using namespace Platformer;

void TestLevel::OnInitialized()
{
	super::OnInitialized();

	LoadMap("TestMap.txt");
}

void TestLevel::Draw()
{
	super::Draw();
}

void TestLevel::LoadMap(const std::string& fileName)
{
	// 최종 경로 조립
	std::string path = std::string("../Assets/") + fileName;

	/* C++ 스타일 */
	// 파일 열기 - 파일 크기와 실제 읽을 바이트 수를 일치시키기 위해 binary 사용
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
	{
		assert(false && "falied to open a sokobatn stage file\n");
		return;
	}
	
	Vector2 position = Vector2::Zero;

	std::string str;
	while (std::getline(file, str))
	{
		levelWidth = str.length() - 1;
		position.x = 0;

		for (int n = 0; n < str.length(); ++n)
		{
			if (str[n] == '\n')
			{
				position.x = 0;
				break;
			}

			levelData.emplace_back(str[n]);
		
			switch (str[n])
			{
			case 'P':
				SpawnActor<Player>(position);
				break;
			case '#':
				SpawnActor<Wall>(position);
				break;
			case '-':
				SpawnActor<Platform>(position);
				break;
			default:
			//case ' ':
				break;
			}

			++position.x;
		}

		++position.y;
	}
	

	levelHeight = position.y;

	
	// 파일 닫기
	file.close();
}

bool TestLevel::CanMove(const Platformer::Vector2& nextPosition)
{
	if (nextPosition.x < 0 || nextPosition.y < 0)
	{
		return false;
	}

	int index = levelWidth * nextPosition.y + nextPosition.x;

	if (levelData[index] != '#' && levelData[index] != '-')
	{
		return true;
	}

	return false;
}
