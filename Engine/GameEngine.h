#pragma once
#include <memory>	// shared_ptr
#include <chrono>	// chrono
#include "UScene.h"		// UScene
#include "UInputSystem.h"	// UInputSystem

namespace Unreal
{
	class GameEngine
	{
	public:
		// 엔진 실행
		void RunLifeCycle()
		{
			// 최대 프레임 제한에 따른 Tick 함수 딜레이 설정
			DWORD frameDelta = (1.f / maxFPS) * 1000;

			// 최초 프레임 시간 기록
			clk = std::chrono::steady_clock::now();

			// Scene에 생성된 액터 초기 설정 진행
			if (scene != nullptr)
			{
				scene->BeginPlay();
			}

			// 반복
			while (this->bIsRunning == true)
			{
				// deltaTime 측정[s]
				auto now = std::chrono::high_resolution_clock::now();	// 현재 프레임의 시간
				float deltaTime = std::chrono::duration<float>(now - clk).count();	// 이전 프레임 사이의 시간간격
				clk = now;	// 시간 갱신

				// 키보드 입력 상태 업데이트
				UInputSystem::UpdateKeyInput();

				// Scene에 생성된 액터 업데이트
				if (scene != nullptr)
				{
					scene->Tick(deltaTime);
				}

				// Scene에 생성된 액터 렌더링
				if (scene != nullptr)
				{
					scene->Render();
				}

				// 화면 딜레이
				Sleep(frameDelta - deltaTime);
			}


			// Scene에 생성된 액터들 일괄 파괴
			if (scene != nullptr)
			{
				scene->EndPlay();
			}
		}

	private:
		// 현재 출력하려는 Scene
		std::shared_ptr<UScene> scene;

		// 무한루프 제어 변수
		bool bIsRunning = true;

		// 이전 프레임의 시간을 저장하는 변수 (deltaTime 측정용)
		std::chrono::steady_clock::time_point clk;

		// 최대 프레임 제한
		uint32_t maxFPS = 60;
	};
}
