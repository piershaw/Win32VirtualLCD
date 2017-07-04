#pragma once
#include "stdafx.h"
#include "Vec2.h"

class Circle
{
public:
	Circle();
	Circle(float R, const Vec2& center);

	bool hits(Circle& A, Vec2& normal);

	float r; // radius
	Vec2  c; // center point
};
