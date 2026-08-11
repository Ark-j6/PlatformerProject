#include "TestLevel.h"
#include <Math/Vector2.h>
#include <Actor/Player.h>
#include <Actor/Terrain.h>
#include <Actor/Platform.h>
#include <Actor/Obstacle.h>

#include <fstream>
#include <cassert>

using namespace Platformer;

void TestLevel::OnInitialized()
{
	super::OnInitialized();

	LoadMap("TestMap.txt");
	maxScreenStartX = levelWidth - screenWidth;
}

void TestLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	int intScreenX = static_cast<int>(screenStartX);

	if (intScreenX < targetScreenX)
	{
		screenStartX += screenSpeed * deltaTime;
	}
	else if (intScreenX > targetScreenX)
	{
		screenStartX -= screenSpeed * deltaTime;
	}
}

void TestLevel::Draw()
{
	for (std::shared_ptr<Actor>& actor : actorList)
	{
		// 검증 - 활성화 되지 않았으면 건너뛰기
		if (!actor->IsActive())
		{
			continue;
		}

		// Screen 영역 내에 있는 actor에 한해서 Draw 이벤트 호출
		int screenEnd = static_cast<int>(screenStartX + screenWidth);
		if (screenStartX <= actor->GetPosition().x && actor->GetPosition().x < screenEnd)
		{
			actor->Draw(static_cast<int>(screenStartX));
		}
	}
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
	bool isActorSpawning = false;
	Platform* current = nullptr;
	unsigned short size = 0;

	std::string str;
	while (std::getline(file, str))
	{
		levelWidth = static_cast<int>(str.length());
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
				SpawnActor<Player>(position);
				levelData.emplace_back(str[n]);
				break;
			case '#':
			case '=':
			{
				std::string s = std::string(1, str[n]);
				SpawnActor<Terrain>(s, position);
				levelData.emplace_back(str[n]);
				break;
			}
			case '*':
				if (!isActorSpawning)
				{
					size = 1;
				}
				else
				{
					assert(current && "* finished, but no current platform");
					current->ChangeWidth(size);
					current->isMovable = true;
					current->isColorChangable = true;
					current = nullptr;
					isActorSpawning = false;
					size = 0;
				}
				levelData.emplace_back(' ');
				break;
			case '(':
			case '{':
			case '[':
			{
				size = 1;
				levelData.emplace_back(' ');
				break;
			}
			case ')':
				assert(current && "* finished, but no current platform");
				current->ChangeWidth(size);
				current->isColorChangable = true;
				current = nullptr;
				size = 0;
				isActorSpawning = false;
				levelData.emplace_back(' ');
				break;
			case '}':
				assert(current && "* finished, but no current platform");
				current->ChangeWidth(size);
				current->isMovable = true;
				current = nullptr;
				size = 0;
				isActorSpawning = false;
				levelData.emplace_back(' ');
				break;
			case ']':
				assert(current && "* finished, but no current platform");
				current->ChangeWidth(size);
				current = nullptr;
				size = 0;
				isActorSpawning = false;
				levelData.emplace_back(' ');
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
				
				levelData.emplace_back('-');
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

				levelData.emplace_back('-');
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

				levelData.emplace_back('-');
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

				levelData.emplace_back('-');
				break;
			case '^':
			case '<':
			case '>':
			case 'v':
			{
				std::string s = std::string(1, str[n]);
				SpawnActor<Obstacle>(s, position);
				levelData.emplace_back(str[n]);
				break;
			}
			default:
				levelData.emplace_back(' ');
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

bool TestLevel::CanMove(const Vector2& nextPosition, Platformer::Color color)
{
	if (nextPosition.x < 0 || screenStartX < 0)
	{
		return false;
	}

	short idx = nextPosition.y * levelWidth + nextPosition.x;
	if (levelData[idx] == '=' || levelData[idx] == '^' || levelData[idx] == '#')
	{
		return false;
	}

	if (levelData[idx] == '-')
	{
		for (std::shared_ptr<Actor>& actor : actorList)
		{
			int posX = actor->GetPosition().x;
			int length = actor->GetImageWidth();
			if (posX >= nextPosition.x && nextPosition.x < posX + length)
			{
				if (actor->GetColor() == color)
				{
					return true;
				}

				return false;
			}
		}
	}


	return true;
}

void TestLevel::CheckPlayerXPos(int playerX)
{
	if (playerX > rightBasePosX)
	{
		targetScreenX += wannaOffsetX;
		rightBasePosX += wannaOffsetX;
		leftBasePosX += wannaOffsetX;

		if (targetScreenX > maxScreenStartX)
		{
			targetScreenX = maxScreenStartX;
		}
	}
	else if (playerX < leftBasePosX)
	{
		targetScreenX -= wannaOffsetX;
		rightBasePosX -= wannaOffsetX;
		leftBasePosX -= wannaOffsetX;

		if (targetScreenX < 0)
		{
			targetScreenX = 0;
			leftBasePosX = 16;
			rightBasePosX = 24;
		}
	}
}

void TestLevel::ChangeActorColors()
{
	for (std::shared_ptr<Actor>& actor : actorList)
	{
		actor->ChangeColor();
	}
}
