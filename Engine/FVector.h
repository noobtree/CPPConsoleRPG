#pragma once
#include <cmath>

// 3차원 좌표를 표현하는 클래스
class FVector
{
public:
	double GetLength()
	{
		double length = std::sqrt((x * x) + (y * y) + (z * z));
		return length;
	}
	double Distance(const FVector& other) const
	{
		double deltaX = this->x - other.x;
		double deltaY = this->y - other.y;
		double deltaZ = this->z - other.z;

		double length = std::sqrt((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ));
		return length;
	}

protected:
	double x, y, z;
};

