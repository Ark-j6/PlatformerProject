#include "Effect.h"

EffectTimer::EffectTimer(float targetTime) : elapsedTime(0.0f), targetTime(targetTime)
{
}

void EffectTimer::Tick(float deltaTime)
{
	// 경과 시간 누적 처리
	elapsedTime += deltaTime;

	// 경과 시간이 목표 시간을 벗어나지 않도록 고정 처리
	elapsedTime = elapsedTime >= targetTime ? targetTime : elapsedTime;
}

void EffectTimer::Reset()
{
	// 경과 시간 초기화
	elapsedTime = 0.0f;
}

void EffectTimer::SetTargetTime(float targetTime)
{
	this->targetTime = targetTime;
}


using namespace Platformer;

Effect::Effect()
{
}

void Effect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 애니메이션 재생을 위한 타이머 업데이트.
	timer.Tick(deltaTime);
	if (!timer.IsTimeOut())
	{
		return;
	}

	// 애니메이션 재생 끝났는지 확인.
	// 끝났으면 삭제.
	if (currentSequenceIndex == effectSequenceCount - 1)
	{
		Destroy();
		return;
	}

	// 타이머 리셋.
	timer.Reset();

	// 이펙트 프레임 업데이트.
	++currentSequenceIndex;

	// 다음 시퀀스에서 재생할 시간으로 타이머 재설정.
	timer.SetTargetTime(sequence[currentSequenceIndex].playTime);

	// 애니메이션 프레임에 사용할 문자열을 액터에 복사.
	ChangeImage(sequence[currentSequenceIndex].frame);

	// 색상 설정.
	color = sequence[currentSequenceIndex].color;
}

void Effect::OnInitiated()
{
	// 애니메이션 시퀀스 개수 구하기.
	effectSequenceCount = static_cast<int>(sequence.size());

	// 다음 애니메이션까지 대기할 시간.
	timer.SetTargetTime(sequence[0].playTime);
}
