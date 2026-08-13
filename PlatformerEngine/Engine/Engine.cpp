#include "Engine.h"
#include <Level/Level.h>
#include <Input/Input.h>
#include <Render/Renderer.h>

#include <Windows.h>	// 타이머를 사용하기 위한 헤더 추가
#include <cassert>		// 싱글톤 검증용 assert 매크로를 사용하기 위한 헤더 추가

#include <fstream>		// C++ 스타일 파일 입출력
#include <string>
#include <cmath>

namespace Platformer
{
	// 전역 변수 초기화
	Engine* Engine::instance = nullptr;

	Engine::Engine()
	{
		// instance 초기화
		assert(!instance && "instance is not null");
		instance = this;

		// 엔진 설정 로드
		LoadEngineSetting();

		// 입력 객체 생성
		input = std::make_unique<Input>();

		// 렌더러 객체 생성
		renderer = std::make_unique<Renderer>(Vector2(setting.width, setting.height));
	}

	Engine::~Engine()
	{
		instance = nullptr;
	}

	void Engine::Run()
	{
		// 고해상도 타이머 사용 - chrono도 있음(ue에서 사용x)

		// 밀리세컨드 - 1/1000초 -> 해상도 1000
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		// 현재 시간 읽기
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		// 프레임 계산을 위한 변수
		int64_t current = counter.QuadPart;
		int64_t prev = current;

		// 고정 프레임으로 만들기 위한 값.
		float oneFrameTime = 1.0f / setting.frameRate;

		// 엔진 루프
		while (true)
		{
			if (isQuit)
			{
				break;
			}

			// 프레임 처리 *************************************

			// 입력 처리
			ProcessInput();

			// 프레임 시간 계산
			// 1. 현재 시간 읽기
			QueryPerformanceCounter(&counter);
			current = counter.QuadPart;

			// 2. (현재 시간 - 이전시간) / 시간 단위(해상도)
			// -> 초단위로 변환 
			// 예) 200밀리세컨트 -> 초?
			float deltaTime = static_cast<float>(current - prev) / static_cast<float>(frequency.QuadPart);

			// 고정 프레임 처리.
			// 프레임 사이에 걸린 시간이 목표 시간보다 더 많이 지났으면 프레임 처리.
			if (deltaTime >= oneFrameTime)
			{
				// 게임 이벤트 함수 호출
				OnInitialized();

				// 게임 이벤트의 초기화 함수(1번만 호출)
				BeginPlay();

				// 게임 업데이트
				Tick(deltaTime);

				// 화면 그리기
				Draw();

				// 프레임 처리 완료 *************************************

				// 레벨 전환 처리
				if (nextLevel)
				{
					// 기존 레벨 정리
					if (mainLevel)
					{
						mainLevel.reset();
					}

					// 추가 요청된 레벨을 메인 레벨로 설정. (스마트 포인터는 = 연산자로 참조 복사? 포인터 전환? 이 되는건가?)
					mainLevel = nextLevel;

					// 포인터 정리
					nextLevel.reset();
				}

				// 추가/제거 요청된 액터 정리
				if (mainLevel)
				{
					mainLevel->ProcessAddAndDestroyActors();
				}

				// 다음 프레임을 위한 입력 상태 저장
				SavePreviousInputStates();

				// 현재 시간을 이전 시간으로 저장
				prev = current;
			}
		}

		// 종료 처리 함수 호출
		ShutDown();
	}

	void Engine::Quit()
	{
		// 엔진 종료 플래그 설정
		isQuit = true;
	}

	Engine& Engine::Get()
	{
		// 검증 - assert
		// 무조건(필수로) 통과해야 하는 조건이 있을 때 사용
		// 디버그 모드에서만 동작
		assert(instance && "");
		return *instance;
	}

	void Engine::ProcessInput()
	{
		assert(input && "input should not be null here");
		if (!input)
		{
			return;
		}

		input->ProcessInput();
	}

	void Engine::OnInitialized()
	{
		// 레벨 초기화 처리
		// 예외처리
		if (!mainLevel || mainLevel->HasInitialized())
		{
			return;
		}

		// 초기화 이벤트 호출
		mainLevel->OnInitialized();
	}

	void Engine::BeginPlay()
	{
		if (!mainLevel)
		{
			return;
		}

		// 레벨에 이벤트 전달
		mainLevel->BeginPlay();
	}

	void Engine::Tick(float deltaTime)
	{
		if (!mainLevel)
		{
			return;
		}

		mainLevel->Tick(deltaTime);
	}

	void Engine::Draw()
	{
		if (!mainLevel)
		{
			return;
		}

		mainLevel->Draw();

		// 렌더러에 Draw 이벤트 호출
		if (!renderer)
		{
			return;
		}

		renderer->Draw();
	}

	void Engine::SavePreviousInputStates()
	{
		assert(input && "input should not be null here");
		if (!input)
		{
			return;
		}

		input->SavePreviousStates();
	}

	void Engine::ShutDown()
	{
	}

	void Engine::LoadEngineSetting()
	{
		/* C++ 스타일 파일 IO*/
		std::ifstream file(configDir);
		assert(file.is_open());
		
		// 라인 별로 읽기
		std::string line;
		while (std::getline(file, line))
		{
			// 빈 줄 및 주석 건너뛰기
			if (line.empty() || line[0] == '#')
			{
				continue;
			}
		
			// key - value 포맷 파싱
			const size_t equalPosition = line.find('=');	// 문자열 내 = 위치 확인
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
			std::string key = line.substr(0, equalPosition);

			// value 파싱 - ' = ' 이후 전체
			std::string value = line.substr(equalPosition + 1);

			// 람다 호출로 key/value에서 공백 제거
			trim(key);
			trim(value);

			// key / value가 제대로 설정됐는지 확인
			assert(!key.empty() && !value.empty());

			// 속성 읽기
			if (key == "framerate")
			{
				setting.frameRate = static_cast<float>(atof(value.c_str()));
				assert(setting.frameRate > 0.0f);
			}
			else if (key == "width")
			{
				setting.width = static_cast<int>(atoi(value.c_str()));
				assert(setting.width > 0);
			}
			else if (key == "height")
			{
				setting.height = static_cast<int>(atoi(value.c_str()));
				assert(setting.height > 0);
			}

			else if (key == "gameViewWidth")
			{
				setting.gameViewWidth = static_cast<int>(atoi(value.c_str()));
				assert(setting.gameViewWidth > 0);
			}
			else if (key == "gameviewHegiht")
			{
				setting.gameviewHegiht = static_cast<int>(atoi(value.c_str()));
				assert(setting.gameviewHegiht > 0);
			}

			else if (key == "gameViewOffsetX")
			{
				setting.gameViewOffsetX = static_cast<int>(atoi(value.c_str()));
				assert(setting.gameViewOffsetX > 0);
			}
			else if (key == "gameViewOffsetY")
			{
				setting.gameViewOffsetY = static_cast<int>(atoi(value.c_str()));
				assert(setting.gameViewOffsetY > 0);
			}

		}

		// 처리가 완료되면 파일 닫기
		file.close();
	}
}