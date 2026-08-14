#pragma once
#include <Core/Core.h>
#include <Core/PlatformerObject.h>
#include <Math/Vector2.h>
#include <Math/Color.h>
#include <memory>		// std::weak_ptr 사용을 위해
#include <string>

namespace Platformer
{
	// 전방 선언
	class Level;

	class PLATFORMER_API Actor : public PlatformerObject
	{
		TYPE_DECLARATIONS(Actor, PlatformerObject)

	public:
		Actor(const std::string& image = ""
			, const Vector2& position = Vector2::Zero
			, Color color = Color::White);
		virtual ~Actor();

		// 게임 플레이 이벤트 함수
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw(int screenX = 0);

		// 액터 제거 함수
		virtual void Destroy();

		// 게임(엔진) 종료 함수
		void QuitGame();


		// Getter/Setter
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }

		// 
		inline std::shared_ptr<Level> GetOwner() const { return owner.lock(); }
		inline void SetOwner(std::weak_ptr<Level> newOwner) { owner = newOwner; }

		inline Vector2 GetPosition() const { return position; }
		void SetPosition(const Vector2& newPosition);

		virtual bool CheckActorPosition(const Vector2& position);

		Color GetColor() const { return color; }
		virtual void ChangeColor();

		inline const std::string GetImage() const { return image; }
		inline int GetImageWidth() const { return width; }

		// 액터의 이미지 설정 함수
		inline void ChangeImage(const std::string& newImage)
		{
			// 이미지 길이 설정
			width = static_cast<int>(newImage.length());

			// 새로운 글자 값 설정
			image = newImage;
		}

	protected:
		// BeginPlay 이벤트 처리 여부 플래그
		bool hasBeganPlay = false;

		// 액터 활성화 여부 플래그
		bool isActive = true;

		// 삭제 요청 여부 플래그
		bool hasExpired = false;

		// 오너십 - 이 액터를 소유하는 레벨 객체
		// weak_ptr (약한 참조) : 실제 사용을 위해서는 해당 위치가 유효한지 확인해야함
		std::weak_ptr<Level> owner;

		// 화면에 그릴 글자
		std::string image;

		// 글자 색상
		Color color = Color::White;

		// 글자 너비
		int width = 0;

		// 렌더링 순서
		int sortingOrder = 0;

		// 위치
		Vector2 position;
	};
}

