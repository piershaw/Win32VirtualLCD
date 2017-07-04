#pragma once
#include "stdafx.h"
#include <windows.h>
#include "Circle.h"
#include "Vec2.h"

class Sprite
{
public:
	Sprite(HINSTANCE hAppInst, int imageID, int maskID,const Vec2& p0);

	~Sprite();

	int width();
	int height();

	void update(float dt);
	void draw(HDC hBackBufferDC, HDC hSpriteDC);

public:
	Vec2      mPosition; 

private:
	// Make copy constructor and assignment operator private
	// so client cannot copy Sprites.  We do this because
	// this class is not designed to be copied because it
	// is not efficient--copying bitmaps is slow (lots of memory).
	Sprite(const Sprite& rhs);
	Sprite& operator=(const Sprite& rhs);

protected:
	HINSTANCE mhAppInst;
	HBITMAP   mhImage;
	HBITMAP   mhMask;
	BITMAP    mImageBM;
	BITMAP    mMaskBM;
};

