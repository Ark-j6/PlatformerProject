#include "PlatformLevel.h"
#include <Game/GameManager.h>
#include <Render/Renderer.h>
#include <Math/Vector2.h>
#include <Actor/Player.h>
#include <Actor/Terrain.h>
#include <Actor/Platform.h>
#include <Actor/Obstacle.h>
#include <Actor/Key.h>
#include <Actor/Door.h>

#include <Windows.h>

#include <fstream>
#include <cassert>

using namespace Platformer;

PlatformLevel::PlatformLevel(const std::string& levelName) : levelName(levelName)
{
}

void PlatformLevel::OnInitialized()
{
	super::OnInitialized();

	screenWidth = Engine::Get().GetScreenWidth();

	LoadMap();
	LoadMapConfig();
	maxScreenStartX = levelWidth - screenWidth;

	levelState = LevelState::Start;
	player->UpdatePlayerInput(true);
}

void PlatformLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	UpdateScreen(deltaTime);

	if (levelState == LevelState::ClearEffect)
	{
		VictoryEffect(deltaTime);
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
				player = SpawnActor<Player>(position);
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
			default:
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

void PlatformLevel::LoadMapConfig()
{
	std::string path = std::string("../Assets/StageConfig/") + levelName;

	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
	{
		assert(false && "falied to open stage file\n");
		return;
	}

	Vector2 pos = Vector2::Zero;
	std::string str;
	size_t prev = 0, current = 0;
	
	PlatformConfig config = {};
	int idx = 0;

	while (std::getline(file, str))
	{
		if (!str.empty() && str.back() == '\r')
			str.pop_back();

		if (str.front() == '[' && str.back() == ']')
		{
			OutputDebugStringA(str.c_str());
			continue;
		}
		else if (!str.empty())
		{
			config = {};
			idx = 0;
			prev = 0;
			while ((current = str.find(',', prev)) != std::string::npos)
			{
				ParsingConfig(config, str.substr(prev, current - prev), idx);
				prev = current + 1;
				++idx;
			}
			ParsingConfig(config, str.substr(prev), idx);
		}

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
	}

}

void PlatformLevel::ParsingConfig(Platformer::PlatformConfig& config, const std::string& value, int configIndex)
{
	switch (configIndex)
	{
	case 0:
		config.basePosition.x = std::stoi(value);
		break;
	case 1:
		config.basePosition.y = std::stoi(value);
		break;
	case 2:
	{
		if (value == "U")
			config.direction = Vector2(0, -1);
		else if (value == "D")
			config.direction = Vector2(0, 1);
		else if (value == "L")
			config.direction = Vector2(-1, 0);
		else							  
			config.direction = Vector2(1, 0);
	}
		break;
	case 3:
		config.distance = std::stoi(value);
		break;
	case 4:
		config.speed = std::stoi(value);
		break;
	default:
		config.isLooping = std::stoi(value) == 0 ? false : true;
		break;
	}
}

void PlatformLevel::UpdateScreen(float deltaTime)
{
	float posX = player.get()->GetPosition().x - screenStartX;

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

void PlatformLevel::VictoryEffect(float deltaTime)
{
	if (victoryEffectTimer < 4.0f)
	{
		victoryEffectTimer += deltaTime;

		int length = static_cast<int>(victory.length());
		size_t x = static_cast<size_t>(length * victoryEffectTimer / 3.f);
		if (x > length)
		{
			x = length;
		}
		std::string str = victory.substr(0, x);
		Renderer::Get().Submit(str, Vector2(static_cast<int>((screenWidth / 2)), 5), Color::Yellow);
	}
	else
	{
		levelState = LevelState::Prepare;
		GameManager& game = dynamic_cast<GameManager&>(Engine::Get());
		game.LoadNextGameLevel();
	}
}

Actor* PlatformLevel::GetActorAt(const Platformer::Vector2& nextPosition)
{
	for (std::shared_ptr<Actor>& actor : actorList)
	{
		if (actor->CheckActorPosition(nextPosition))
		{
			if (actor->IsTypeOf<Player>())
			{
				continue;
			}

			return actor.get();
		}
	}

	return nullptr;
}

Actor* PlatformLevel::GetActorByRequestList(const Platformer::Vector2& nextPosition)
{
	for (std::shared_ptr<Actor>& actor : addRequestedActorList)
	{
		if (actor->CheckActorPosition(nextPosition))
		{
			if (actor->IsTypeOf<Player>())
			{
				continue;
			}

			return actor.get();
		}
	}

	return nullptr;
}

bool PlatformLevel::CanMove(const Actor* other, Color color)
{
	if (other)
	{
		if (other->IsTypeOf<Terrain>() || other->IsTypeOf<Obstacle>())
		{
			return false;
		}
		if (other->IsTypeOf<Key>() || other->IsTypeOf<Door>())
		{
			return true;
		}
		else if (other->GetColor() != color)
		{
			return false;
		}
	}

	return true;
}

void PlatformLevel::HandleInteraction(Actor* target, const Platformer::Vector2& direction)
{
	assert(target && "No Actor for interaction");

	if (target->IsTypeOf<Key>())
	{
		target->Destroy();
		player.get()->GetKey();
	}
	else if (target->IsTypeOf<Door>())
	{
		if (player.get()->HasKey())
		{
			victoryEffectTimer = 0.0f;
			player->UpdatePlayerInput(false);
			levelState = LevelState::ClearEffect;
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

		}
	}
	else if (target->IsTypeOf<Platform>())
	{
		player.get()->UpdateStandingPlatform(dynamic_cast<Platform*>(target));
	}
}

void PlatformLevel::ChangeActorColors()
{
	for (std::shared_ptr<Actor>& actor : actorList)
	{
		actor->ChangeColor();
	}
}
