#pragma once
#include "Bullet.h"
class CMonstroBullet_1 :
	public CBullet
{
public:
	CMonstroBullet_1();
	~CMonstroBullet_1();

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

