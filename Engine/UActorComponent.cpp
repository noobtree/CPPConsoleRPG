#include "UActorComponent.h"
#include "AActor.h"

// 소유자(액터)로부터 컴포넌트(자신)을 제거하는 함수
void Unreal::UActorComponent::Destroy()
{
	// 소유자(액터) 유효성 확인
	std::shared_ptr<AActor> Owner = owner.lock();
	if (Owner != nullptr)
	{
		bool bIsDestroy = Owner->DettachComponent(shared_from_this());
	}
}

// BeginPlay가 이전에 호출되지 않았을 경우에 BeginPlay를 호출하는 함수
void Unreal::UActorComponent::OnBeginPlay()
{
	if (bIsBeginCalled == true)
	{
		return;
	}

	BeginPlay();
	bIsBeginCalled = true;
}

// BeginPlay가 이전에 호출되었을 경우에 Tick을 호출하는 함수
void Unreal::UActorComponent::OnTick(float deltaTime)
{
	if (bIsBeginCalled == false)
	{
		OnBeginPlay();
		return;
	}

	Tick(deltaTime);
}

// BeginPlay가 이전에 호출되었을 경우에 EndPlay를 호출하는 함수
void Unreal::UActorComponent::OnEndPlay()
{
	if (bIsBeginCalled == false)
	{
		return;
	}

	EndPlay();
	bIsBeginCalled = false;
}
