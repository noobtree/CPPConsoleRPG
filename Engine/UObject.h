#pragma once
#include <memory>
#include <boost/uuid/uuid.hpp>		// uuid
#include <boost/uuid/uuid_io.hpp>	// stream input output
//#include <boost/lexical_cast.hpp>	// to_string

namespace Unreal
{
	// 모든 객체가 상속받는 기본 클래스
	class UObject
	{
	public:
		UObject();
		virtual ~UObject() = default;

		// 비교 함수 재정의
		bool operator==(const UObject& other);
		bool operator!=(const UObject& other);

	private:
		// 인스턴스의 고유 아이디
		boost::uuids::uuid uid;
	};
}
