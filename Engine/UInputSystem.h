#pragma once
#include <Windows.h>
#include <cstdint>
#include "UObject.h"	// UObject

namespace Unreal
{
	// 입력 키 매핑한 열거형
	enum class EKeyboard : int32_t
	{
		A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		Zero = '0', One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		Up = VK_UP,Down = VK_DOWN, Left = VK_LEFT, Right = VK_RIGHT, Space = VK_SPACE,
		LeftShift = VK_LSHIFT, LeftControl = VK_LCONTROL, LeftAlt = VK_LMENU,
		RightShift = VK_RSHIFT, RightControl = VK_RCONTROL, RightAlt = VK_RMENU,
		Esc = VK_ESCAPE, Tab = VK_TAB, Capslock = VK_CAPITAL,
		BackSpace = VK_BACK, BackSlash = VK_OEM_5, Return = VK_RETURN,
	};

	// 이전 프레임과 현재 프레임에 대하여 키보드 입력 상태를 관리하는 클래스
	class UInputSystem : public UObject
	{
		friend class GameEngine;
	public:
		// 키보드 버튼을 누른 시점(1프레임) 동안 true 반환
		static bool GetKeyDown(EKeyboard keyCode);
		// 키보드 버튼을 누르고 있는 모든 프레임에서 true 반환
		static bool GetKey(EKeyboard keyCode);
		// 키보드 버튼을 놓은 시점(1프레임) 동안 true 반환
		static bool GetKeyUp(EKeyboard keyCode);

	private:
		// 다른 창을 띄우고 있을 때, UpdateKeyInput 함수 내부의 반복문을 줄이기 위한 플래그
		static bool bNeedUpdate;

		// 이전 프레임의 키 입력 상태
		static bool previousInput[256];

		// 현재 프레임의 키 입력 상태
		static bool currentInput[256];

		// 이전 프레임과 현재 프레임에 대한 키 입력 상태를 갱신
		static void UpdateKeyInput();
	};
}
