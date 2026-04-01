#pragma once
#include <vector>	// vector
#include <memory>	// shared_ptr
#include <string>	// string
#include "UObject.h"	// UObject
#include "UActorComponent.h"	// UActorComponent
#include "FVector.h"	// FVector

namespace Unreal
{
	// Outliner에 배치되어 화면에 출력되는 객체들의 추상 클래스
	class AActor : public UObject, public std::enable_shared_from_this<AActor>
	{
		friend class UScene;
	public:
		virtual ~AActor()
		{
			// 컴포넌트 목록 초기화
			for (auto& component : components)
			{
				component.reset();
			}
			components.clear();
		}

		// 런타임 중 최초에 1회 실행
		// call after components.BeginPlay
		virtual void BeginPlay() = 0;

		/// <summary>
		/// 런타임 중 매 프레임마다 실행
		/// <para>call after components.Tick</para>
		/// </summary>
		/// <param name="delaTime">이전 프레임 사이의 시간 간격</param>
		virtual void Tick(float deltaTime) = 0;

		// 런타임 중 최후에 1회 실행
		// call after components.EndPlay
		virtual void EndPlay() = 0;

		// 화면에 객체를 출력
		virtual void Render() = 0;

		/// <summary>
		/// 임의 타입의 컴포넌트를 액터에 추가
		/// </summary>
		/// <param name="component"></param>
		/// <returns>nullptr : 추가된 컴포넌트 없음</returns>
		template <typename TComponent>
		std::shared_ptr<TComponent> AttachComponent(const std::shared_ptr<TComponent> component = nullptr)
		{
			// 컴파일 시점 template 타입 검사
			static_assert(std::is_base_of<UActorComponent, TComponent>::value, "TComponent must be derived from UActorComponent class.");

			// 인스턴스 생성
			std::shared_ptr<UActorComponent> componentInstance = component == nullptr ? std::make_shared<TComponent>() : component;

			// 컴포넌트의 소유자 설정
			componentInstance->owner = shared_from_this();

			// 컴포넌트 목록에 추가
			components.push_back(componentInstance);

			// 포인터 형 변환 후 반환
			return std::dynamic_pointer_cast<TComponent>(componentInstance);
		}

		/// <summary>
		/// 임의 타입의 컴포넌트를 찾아서 반환
		/// </summary>
		/// <returns>nullptr : 해당 타입의 컴포넌트 없음</returns>
		template <typename TComponent>
		std::shared_ptr<TComponent> GetComponent()
		{
			// 컴파일 시점 template 타입 검사
			static_assert(std::is_base_of<UActorComponent, TComponent>::value, "TComponent must be derived from UActorComponent class.");

			// 컴포넌트 목록 검색
			for (auto it = components.begin(); it != components.end(); ++it)
			{
				// nullptr 참조 방어
				if (*it != nullptr)
				{
					// 동적 타입이 동일한지 검사
					if (typeid((*it).get()) == typeid(TComponent))
					{
						// 포인터 형 변환 후 반환
						return std::dynamic_pointer_cast<TComponent>(*it);
					}
				}
			}

			return nullptr;
		}

		/// <summary>
		/// 임의 타입의 컴포넌트를 액터에서 제거
		/// </summary>
		/// <returns>True : 제거 완료됨</returns>
		template<typename TComponent>
		bool DettachComponent(const std::shared_ptr<TComponent> component = nullptr)
		{
			// 컴파일 시점 template 타입 검사
			static_assert(std::is_base_of<UActorComponent, TComponent>::value, "TComponent must be derived from UActorComponent class.");

			// 컴포넌트 목록 검색
			for (auto& componentInstance : components)
			{
				// nullptr 검사 & 동적 타입 검사
				if ((componentInstance != nullptr) && (typeid(componentInstance.get()) == typeid(TComponent)))
				{
					// 특정 인스턴스를 또는 동일한 타입의 임의 인스턴스를 목표로 하는지 확인
					if (component == nullptr || component == componentInstance)
					{
						// 컴포넌트의 OnEndPlay 호출
						componentInstance->OnEndPlay();

						// 컴포넌트를 가리키는 포인터의 참조 카운트 감소
						componentInstance.reset();

						// 컴포넌트 목록 갱신 플래그 설정
						bIsComponentsChanged = true;

						return true;
					}
				}
			}
			return false;
		}

	protected:
		// 액터의 이름
		std::string name = "MyActor";

		// 액터의 위치 좌표
		FVector location;

		// 현재 엑터에 추가된 컴포넌트 목록
		std::vector<std::shared_ptr<UActorComponent>> components;

	private:
		// BeginPlay가 호출되었는지 확인하기 위한 변수
		bool bIsBeginCalled = false;

		// 액터에 붙은 컴포넌트 목록이 갱신되었는지 확인하는 변수
		bool bIsComponentsChanged = false;

		// BeginPlay가 이전에 호출되지 않았을 경우에 BeginPlay를 호출하는 함수
		void OnBeginPlay();

		// BeginPlay가 이전에 호출되었을 경우에 Tick을 호출하는 함수
		void OnTick(float deltaTime);

		// BeginPlay가 이전에 호출되었을 경우에 EndPlay를 호출하는 함수
		void OnEndPlay();
	};
}
