#pragma once
#include "Bullet.h"
class CPlayerBullet :
	public CBullet
{
public:
	CPlayerBullet();
	~CPlayerBullet();
public:
	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;

private:
	virtual void Release() override;
public:
	void SetBulletType(wstring _bullet_type);
	void SetImageSize(POINT pt);
	void SetColSize(float x,float y);
private:
	wstring bullet_type;
	int shadowY = 0;
	bool testbool;
};

