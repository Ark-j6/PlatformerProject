#pragma once
#include <Actor/Actor.h>
#include <vector>

//#include <string>
//#include <Math/Color.h>

// 애니메이션 이펙트 프레임 구조체.
struct EffectFrame
{
	EffectFrame(
		const std::string& frame,
		float playTime = 0.05f,
		Platformer::Color color = Platformer::Color::Red)
		: frame(frame), playTime(playTime), color(color)
	{
	}

	~EffectFrame() = default;

	// 문자열 변수 (화면에 보여줄 문자열).
	std::string frame;

	// 재생 시간.
	float playTime = 0.0f;

	// 색상.
	Platformer::Color color = Platformer::Color::White;
};

class EffectTimer
{
public:
	EffectTimer(float targetTime = 1.0f);

	// 타이머 시간 업데이트 함수
	void Tick(float deltaTime);

	// 경과시간 리셋 함수
	void Reset();

	// 목표 시간 재설정 함수
	void SetTargetTime(float targetTime);

	// 설정한 시간이 지났는지 확인하는 함수
	inline bool IsTimeOut() const { return elapsedTime >= targetTime; }

private:
	// 경과 시간 계산용 변수
	float elapsedTime = 0.0f;

	// 타이머 목표 시간
	float targetTime = 0.0f;
};

namespace Platformer
{
	class Effect : public Actor
	{
		TYPE_DECLARATIONS(Effect, Actor)
	public:
		Effect();
		~Effect() = default;

	protected:
		virtual void Tick(float deltaTime) override;
		void OnInitiated();

	protected:
		// 시퀀스 문자열 수(배열 원소 수).
		int effectSequenceCount = 0;

		// 현재 보여지는 시퀀스 인덱스.
		int currentSequenceIndex = 0;

		// 애니메이션 재생에 사용할 타이머.
		// 시퀀스 사이에 시간 계산용.
		EffectTimer timer;

		std::vector<EffectFrame> sequence;
	};
}