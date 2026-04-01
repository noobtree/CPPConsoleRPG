#pragma once
#include <vector>	// vector
#include <memory>	// shared_ptr
#include <string>	// string
#include "AActor.h"		// AActor
#include "UObject.h"	// UObject

namespace Unreal
{
	// 하나의 장면을 관리하는 객체
	// 상속 금지
	class UScene : public UObject
	{
	public:
		// 게임 실행 전 최초 1회 실행
		void BeginPlay();

		// 게임 실행 중 매 프레임마다 실행
		void Tick(float deltaTime);

		// 게임 종료 전 1회 실행
		void EndPlay();

		// 게임 실행 중 매 프레임마다 실행
		void Render();

		/// <summary>
		/// Scene에 액터를 생성 또는 등록하는 함수
		/// </summary>
		/// <param name="actor">nullptr : 새로운 인스턴스 생성 및 등록</param>
		/// <returns>아웃라이너에 등록된 액터</returns>
		template <typename TActor>
		std::shared_ptr<TActor> Spawn(std::shared_ptr<TActor> actor = nullptr)
		{
			// 컴파일 시점 template 타입 검사
			static_assert(std::is_base_of<AActor, TActor>::value, "TActor must be derived from AActor class.");

			// nullptr 검사 및 인스턴스 생성
			std::shared_ptr<AActor> actorInstance = (actor == nullptr) ? std::make_shared<TActor>() : actor;

			// outliner에 액터 인스턴스 추가
			outliner.push_back(actorInstance);

			// 포인터 형 변환 후 반환
			return std::dynamic_pointer_cast<TActor>(actorInstance);
		}
		
		/// <summary>
		/// Scene에 생성된 액터를 삭제하는 함수
		/// </summary>
		/// <param name="actor"></param>
		/// <returns></returns>
		template <typename TActor>
		bool Destroy(std::shared_ptr<AActor> actor)
		{
			// 컴파일 시점 template 타입 검사
			static_assert(std::is_base_of<AActor, TActor>::value, "TActor must be derived from AActor class.");

			// nullptr 검사
			if (actor != nullptr)
			{
				// 아웃라이너 검색
				for (auto& actorInstance : outliner)
				{
					// 목표한 액터와 동일하지 확인
					if (actor == actorInstance)
					{
						// 액터의 OnEndPlay 호출
						actorInstance->OnEndPlay();

						// 액터를 가리키는 포인터의 참조 카운트 감소
						actorInstance.reset();

						// 아웃라이너 목록 갱신 플래그 설정
						bIsOutlinerChanged = true;
					}
				}
			}

			return false;
		}

		bool operator==(const UScene& other);
		bool operator!=(const UScene& other);

	private:
		// 아웃라이너가 갱신할 필요가 있는지 확인하는 변수
		bool bIsOutlinerChanged = false;

		// 아웃라이너
		std::vector<std::shared_ptr<AActor>> outliner;

		// Scene 이름
		std::string name;
	};
}
