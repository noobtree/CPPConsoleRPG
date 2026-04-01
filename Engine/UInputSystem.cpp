#include "UInputSystem.h"

// 다른 창을 띄우고 있을 때, UpdateKeyInput 함수 내부의 반복문을 줄이기 위한 플래그
bool Unreal::UInputSystem::bNeedUpdate = true;

// 이전 프레임의 키 입력 상태
bool Unreal::UInputSystem::previousInput[256] = { false };

// 현재 프레임의 키 입력 상태
bool Unreal::UInputSystem::currentInput[256] = { false };

// 키보드 버튼을 누른 시점(1프레임) 동안 true 반환
bool Unreal::UInputSystem::GetKeyDown(EKeyboard keyCode)
{
	int index = static_cast<int>(keyCode);
	if (previousInput[index] == false)
	{
		return currentInput[index];
	}
	return false;
}

// 키보드 버튼을 누르고 있는 모든 프레임에서 true 반환
bool Unreal::UInputSystem::GetKey(EKeyboard keyCode)
{
	int index = static_cast<int>(keyCode);
	if (previousInput[index] == true)
	{
		return currentInput[index];
	}
	return false;
}

// 키보드 버튼을 놓은 시점(1프레임) 동안 true 반환
bool Unreal::UInputSystem::GetKeyUp(EKeyboard keyCode)
{
	int index = static_cast<int>(keyCode);
	if (previousInput[index] == true)
	{
		return currentInput[index] == false;
	}
	return false;
}

// 이전 프레임과 현재 프레임에 대한 키 입력 상태를 갱신
void Unreal::UInputSystem::UpdateKeyInput()
{
	// 현재 최 상단에 위치한 창 정보 얻어오기
	HWND hwnd = GetForegroundWindow();

	// 현재 프로그램이 활성화된 창인지 확인
	if (hwnd != GetConsoleWindow() && hwnd != GetActiveWindow())
	{
		// 업데이트 필요 확인
		if (bNeedUpdate == true)
		{
			// 이전, 현재 프레임에 대하여 각각의 키 입력 상태를 갱신
			for (int i = 0; i < 256; ++i)
			{
				// 이전 프레임의 키 입력 상태를 갱신
				previousInput[i] = false;
				// 현재 프레임의 키 입력 상태를 갱신
				currentInput[i] = false;
			}
			bNeedUpdate = false;
		}
		return;
	}

	// 이전, 현재 프레임에 대하여 각각의 키 입력 상태를 갱신
	for (int i = 0; i < 256; ++i)
	{
		// 이전 프레임의 키 입력 상태를 previousInput에 복사
		previousInput[i] = currentInput[i];

		// 현재 프레임의 키 입력 상태를 갱신
		currentInput[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
	}
	bNeedUpdate = true;
}
