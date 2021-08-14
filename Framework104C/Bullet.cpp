#include "stdafx.h"
#include "Bullet.h"


CBullet::CBullet()
{
}


CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{

}

int CBullet::Update()
{
	return 0;
}

void CBullet::Render(HDC hDC)
{

}

void CBullet::Release()
{
}
void CBullet::SetMovePos(float x, float y)
{
	moveX = x;
	moveY = y;
}

void CBullet::SetRange(float _range)
{
	range = _range;
}

void CBullet::SetAniFlag(bool flag)
{
	aniFlag = flag;
}
