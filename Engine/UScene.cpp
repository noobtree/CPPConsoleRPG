#include "UScene.h"

// 게임 실행 전 최초 1회 실행
void Unreal::UScene::BeginPlay()
{
	for (const auto& actor : outliner)
	{
		if (actor != nullptr)
		{
			actor->OnBeginPlay();
		}
	}
}

// 게임 실행 중 매 프레임마다 실행
void Unreal::UScene::Tick(float deltaTime)
{
	// 컴포넌트 목록을 갱신해야하는지 확인
	if (bIsOutlinerChanged == true)
	{
		// 유효한 액터들을 보관할 버퍼
		std::vector<std::shared_ptr<AActor>> buffer;
		buffer.reserve(outliner.size());

		// 기존 아웃라이너에 등록된 액터들 중 nullptr를 제외하고 옮기기
		for (int i = 0; i < outliner.size(); ++i)
		{
			if (outliner[i] != nullptr)
			{
				buffer.push_back(outliner[i]);
				outliner[i]->OnTick(deltaTime);
			}
		}

		// 아웃라이너 교체
		outliner = buffer;
	}

	for (const auto& actor : outliner)
	{
		if (actor != nullptr)
		{
			actor->OnTick(deltaTime);
		}
	}
}

// 게임 종료 전 1회 실행
void Unreal::UScene::EndPlay()
{
	for (const auto& actor : outliner)
	{
		if (actor != nullptr)
		{
			actor->OnEndPlay();
		}
	}
}

// 게임 실행 중 매 프레임마다 실행
void Unreal::UScene::Render()
{
	for (const auto& actor : outliner)
	{
		if (actor != nullptr)
		{
			actor->Render();
		}
	}
}

bool Unreal::UScene::operator==(const UScene& other)
{
	if (typeid(this) == typeid(other))
	{
		if (this->name == other.name)
		{
			return true;
		}
	}
	return false;
}

bool Unreal::UScene::operator!=(const UScene& other)
{
	if (typeid(this) == typeid(other))
	{
		if (this->name == other.name)
		{
			return false;
		}
	}
	return true;
}
