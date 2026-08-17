#include "PlatformLevel.h"
#include <Game/GameManager.h>
#include <Math/Vector2.h>
#include <Actor/Player.h>
#include <Actor/Terrain.h>
#include <Actor/Platform.h>
#include <Actor/Obstacle.h>
#include <Actor/Key.h>
#include <Actor/Door.h>
#include <Actor/CheckPoint.h>
#include <Actor/DeadZone.h>
#include <Effect/CheckpointEffect.h>
#include <Effect/DeathEffect.h>
#include <Effect/VictoryEffect.h>

#include <fstream>
#include <cassert>

using namespace Platformer;

PlatformLevel::PlatformLevel(const std::string& levelName) : levelName(levelName)
{
}

void PlatformLevel::OnInitialized()
{
	super::OnInitialized();

	ResetLevel();
}

void PlatformLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (levelState == LevelState::Start)
	{
		UpdateScreen(deltaTime);
	}
}

void PlatformLevel::Draw()
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
			int pivot = static_cast<int>(screenStartX - Engine::Get().GetXOffset());
			actor->Draw(pivot);
		}
	}
}

void PlatformLevel::LoadMap()
{
	// 최종 경로 조립
	std::string path = std::string("../Assets/Stages/") + levelName;

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
				break;
			case '(':
			case '{':
			case '[':
			{
				size = 1;
				break;
			}
			case ')':
				assert(current && "* finished, but no current platform");
				current->ChangeWidth(size);
				current->isColorChangable = true;
				current = nullptr;
				size = 0;
				isActorSpawning = false;
				break;
			case '}':
				assert(current && "* finished, but no current platform");
				current->ChangeWidth(size);
				current->isMovable = true;
				current = nullptr;
				size = 0;
				isActorSpawning = false;
				break;
			case ']':
				assert(current && "* finished, but no current platform");
				current->ChangeWidth(size);
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
			case '^':
			case '<':
			case '>':
			case 'v':
			{
				std::string s = std::string(1, str[n]);
				SpawnActor<Obstacle>(s, position);
			}
				break;
			case 'K':
				SpawnActor<Key>(position);
				break;
			case 'M':
				SpawnActor<Door>(position);
				break;
			case 'C':
				SpawnActor<CheckPoint>(position);
				break;
			case 'D':
				SpawnActor<DeadZone>(position);
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

void PlatformLevel::LoadMapConfig()
{
	std::string path = std::string("../Assets/StageConfig/") + levelName;

	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
	{
		//assert(false && "falied to open stage file\n");
		return;
	}

	std::string str;
	PlatformConfig config = {};
	bool canRead = false;

	while (std::getline(file, str))
	{
		if (!str.empty() && str.back() == '\r')
		{
			str.pop_back();
		}

		if (str.find_first_not_of(" \t") == std::string::npos)
		{
			continue;
		}

		if (str.front() == '&')
		{
			if (Actor* actor = GetActorByRequestList(config.basePosition))
			{
				if (MovableObject* mo = dynamic_cast<MovableObject*>(actor))
				{
					mo->SetMovableConfig(config);
				}
				else
				{
					assert(false && "MovalbeObject error\n");
				}
			}
			else
			{
				assert(false && "basePostion error\n");
			}

			canRead = false;
		}

		if (str.front() == '[' && str.back() == ']')
		{
			canRead = true;
			config = {};
			continue;
		}
		else if (!str.empty() && canRead)
		{
			//주석 건너뛰기
			if (str[0] == '#')
			{
				continue;
			}

			ParsingConfig(config, str);
		}
	}

}

void PlatformLevel::ParsingConfig(PlatformConfig& config, const std::string& str)
{
	// key - value 포맷 파싱
	const size_t equalPosition = str.find('=');	// 문자열 내 = 위치 확인
	assert(equalPosition != std::string::npos);

	// 람다식을 이용해서 문자열을 검색하도록
	auto trim = [](std::string& s)
	{
		// 공백 문자 집합
		// ' '	: 스페이스(공백)
		// \t	: 탭
		// \r	: 윈도우 개행문자 일자
		// \n	: 개행 문자
		const char* whiteSpace = " \t\r\n";

		// 문자열의 앞에서부터 공백이 아닌 첫 문자 위치 검색
		const size_t begin = s.find_first_not_of(whiteSpace);

		// 공백이 아닌 문자를 못찾은 경우에는 빈 문자열로 설정 후 변환
		if (begin == std::string::npos)
		{
			s.clear();
			return;
		}

		// 문자열의 뒤에서부터 공백이 아닌 마지막 문자 위치 검색
		const size_t end = s.find_last_not_of(whiteSpace);
		s = s.substr(begin, end - begin + 1);
		// 예) s = " abc "  -> begin = 1 (a의 index), end = 3 (c의 index)
	};

	// key 파싱
	std::string key = str.substr(0, equalPosition);

	// value 파싱 - ' = ' 이후 전체
	std::string value = str.substr(equalPosition + 1);

	// 람다 호출로 key/value에서 공백 제거
	trim(key);
	trim(value);

	// key / value가 제대로 설정됐는지 확인
	assert(!key.empty() && !value.empty());

	// 속성 읽기
	if (key == "x")
	{
		config.basePosition.x = stoi(value);
	}
	else if (key == "y")
	{
		config.basePosition.y = stoi(value);
	}
	else if (key == "direction")
	{
		Vector2 dir = Vector2::Zero;

		if (value == "U")
		{
			dir.y = -1;
		}
		else if (value == "D")
		{
			dir.y = 1;
		}
		else if (value == "L")
		{
			dir.x = -1;
		}
		else
		{
			dir.x = 1;
		}

		config.direction = dir;
	}
	else if (key == "distance")
	{
		config.distance = stoi(value);
	}
	else if (key == "speed")
	{
		config.speed = stoi(value);
	}
	else if (key == "looping")
	{
		config.isLooping = value == "1";
	}
	else if (key == "autoColor")
	{
		config.autoColor = stoi(value);
	}
}

void PlatformLevel::UpdateScreen(float deltaTime)
{
	float posX = player->GetPosition().x - screenStartX;

	if (posX > rightBaseScreenPosX)
	{
		screenStartX += screenSpeed * deltaTime;
	}
	else if (posX < leftBaseScreenPosX)
	{
		screenStartX -= screenSpeed * deltaTime;
	}

	if (screenStartX < 0.0f)
	{
		screenStartX = 0;
	}
	else if (screenStartX > maxScreenStartX)
	{
		screenStartX = static_cast<float>(maxScreenStartX);
	}
}

void PlatformLevel::Ouch()
{
	player->UpdatePlayerInput(false);
	levelState = LevelState::DeathEffect;
}

void PlatformLevel::HandleInteraction(Actor* target, const Vector2& direction)
{
	assert(target && "No Actor for interaction");

	if (target->IsTypeOf<Key>())
	{
		target->Destroy();
		player->GetKey();
	}
	else if (target->IsTypeOf<Door>())
	{
		bool isThereKey = false;

		for (auto& key : actorList)
		{
			if (key->IsTypeOf<Key>())
			{
				isThereKey = true;
			}
		}

		if (!isThereKey || player->HasKey())
		{
			levelState = LevelState::ClearEffect;
			player->Destroy();
			SpawnActor<VictoryEffect>(Vector2(static_cast<int>(screenStartX + (screenWidth / 2)), 5));
		}
	}
	else if (target->IsTypeOf<Obstacle>())
	{
		const std::string image = target->GetImage();
		if ((image == "^" && direction.x == 0 && direction.y == 1)
			|| (image == ">" && direction.x == -1 && direction.y == 0)
			|| (image == "<" && direction.x == 1 && direction.y == 0)
			|| (image == "v" && direction.x == 0 && direction.y == -1)
			)
		{
			levelState = LevelState::DeathEffect;
			lastPlayerColor = player->GetColor();
			player->Destroy();
			SpawnActor<DeathEffect>(player->GetPosition());
			player = nullptr;
		}
	}
	else if (target->IsTypeOf<Platform>())
	{
		player->UpdateStandingPlatform(dynamic_cast<Platform*>(target));
	}
	else if (target->IsTypeOf<CheckPoint>())
	{
		currentCheckPoint = target->GetPosition();
		target->Destroy();
		SpawnActor<CheckpointEffect>(target->GetPosition());
	}
	else if (target->IsTypeOf<DeadZone>())
	{
		levelState = LevelState::DeathEffect;
		lastPlayerColor = player->GetColor();
		player->Destroy();
		SpawnActor<DeathEffect>(player->GetPosition());
		player = nullptr;
	}
}

void PlatformLevel::RespawnPlayer()
{
	levelState = LevelState::Start;
	player = SpawnActor<Player>(currentCheckPoint, lastPlayerColor);
}

void PlatformLevel::RequestNextLevel()
{
	levelState = LevelState::Prepare;
	GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
	game.LoadNextGameLevel();
}

void PlatformLevel::ResetLevel()
{
	screenWidth = Engine::Get().GetScreenWidth();

	LoadMap();
	LoadMapConfig();
	maxScreenStartX = levelWidth - screenWidth;

	currentCheckPoint = player->GetPosition();
	levelState = LevelState::Start;
}
