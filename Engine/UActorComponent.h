#pragma once
#include <memory>	// shared_ptr
#include <string>	// string
#include "UObject.h"	// UObject

namespace Unreal
{
	// Forward Declaration
	class AActor;

	// AActor에 추가할 수 있는 컴포넌트의 추상 클래스
	// UActorComponent를 상속받는 클래스는 생성자 정의 금지
	class UActorComponent : public UObject, public std::enable_shared_from_this<UActorComponent>
	{
		friend class AActor;
	public:
		virtual ~UActorComponent()
		{
			owner.reset();
		}

		// 런타임 중 최초에 1회 실행
		virtual void BeginPlay() = 0;

		/// <summary>
		/// 런타임 중 매 프레임마다 실행
		/// </summary>
		/// <param name="delaTime">이전 프레임 사이의 시간 간격</param>
		virtual void Tick(float deltaTime) = 0;

		// 런타임 중 최후에 1회 실행
		virtual void EndPlay() = 0;

		inline std::shared_ptr<AActor> GetOwner() const
		{
			return owner.lock();
		}

		// 엑터로부터 컴포넌트(자신)을 제거하는 함수
		void Destroy();

	protected:
		// 인스턴스를 소유하고 있는 AActor를 저장하는 변수
		std::weak_ptr<AActor> owner;	// 순환 참조 방지를 위하여 weak_ptr 사용

	private:
		// BeginPlay가 호출되었는지 확인하기 위한 변수
		bool bIsBeginCalled = false;

		// BeginPlay가 이전에 호출되지 않았을 경우에 BeginPlay를 호출하는 함수
		void OnBeginPlay();

		// BeginPlay가 이전에 호출되었을 경우에 Tick을 호출하는 함수
		void OnTick(float deltaTime);

		// BeginPlay가 이전에 호출되었을 경우에 EndPlay를 호출하는 함수
		void OnEndPlay();
	};
}