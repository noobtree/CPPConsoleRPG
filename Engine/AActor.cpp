#include "AActor.h"

void Unreal::AActor::OnBeginPlay()
{
	// 액터의 BeginPlay가 호출되었는지 확인
	if (bIsBeginCalled == true)
	{
		return;
	}

	// 부착된 컴포넌트의 BeginPlay 호출
	for (const auto& component : components)
	{
		if (component != nullptr)
		{
			component->OnBeginPlay();
		}
	}

	// 액터의 BeginPlay 호출
	BeginPlay();

	// 플래그 값 갱신
	bIsBeginCalled = true;
}

void Unreal::AActor::OnTick(float deltaTime)
{
	// 액터의 BeginPlay가 호출되었는지 확인
	if (bIsBeginCalled == false)
	{
		// 액터의 BeginPlay 호출
		OnBeginPlay();
		return;
	}


	// 컴포넌트 목록을 갱신해야하는지 확인
	if (bIsComponentsChanged == true)
	{
		// 유효한 컴포넌트를 보관할 버퍼
		std::vector<std::shared_ptr<UActorComponent>> buffer;
		buffer.reserve(components.size());

		// 기존 컴포넌트 목록에서 nullptr를 제외하고 옮기기
		for (int i = 0; i < components.size(); ++i)
		{
			if (components[i] != nullptr)
			{
				buffer.push_back(components[i]);
				components[i]->OnTick(deltaTime);
			}
		}

		// 컴포넌트 목록 교체
		components = buffer;
	}
	else
	{
		// 부착된 컴포넌트의 Tick 호출
		for (const auto& component : components)
		{
			if (component != nullptr)
			{
				component->OnTick(deltaTime);
			}
		}
	}

	// 액터의 Tick 호출
	Tick(deltaTime);
}

void Unreal::AActor::OnEndPlay()
{
	// 액터의 BeginPlay가 호출되었는지 확인
	if (bIsBeginCalled == false)
	{
		return;
	}

	// 부착된 컴포넌트의 EndPlay 호출
	for (const auto& component : components)
	{
		if (component != nullptr)
		{
			component->OnEndPlay();
		}
	}

	// 액터의 EndPlay 호출
	EndPlay();

	// 플래그 값 갱신
	bIsBeginCalled = false;
}
