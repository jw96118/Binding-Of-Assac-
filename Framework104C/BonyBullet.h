#pragma once
#include "Bullet.h"
class CBonyBullet :
	public CBullet
{
public:
	CBonyBullet();
	~CBonyBullet();

	// CGameObject��(��) ���� ��ӵ�
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

