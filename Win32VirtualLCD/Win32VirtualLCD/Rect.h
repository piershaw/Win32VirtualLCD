#pragma once
#include "stdafx.h"
#include "Circle.h"
#include "Vec2.h"

class Rect
{
public:
	Rect();
	Rect(const Vec2& a, const Vec2& b);
	Rect(float x0, float y0, float x1, float y1);

	void forceInside(Circle& A);
	bool isPtInside(const Vec2& pt);

	Vec2 minPt;
	Vec2 maxPt;
};

