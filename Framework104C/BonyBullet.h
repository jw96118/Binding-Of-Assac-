#pragma once
#include "Bullet.h"
class CBonyBullet :
	public CBullet
{
public:
	CBonyBullet();
	~CBonyBullet();

	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;

private:
	virtual void Release() override;
private:
	float w;
	float h;
	float d;
	int breakCount;
};

