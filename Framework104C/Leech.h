#pragma once
#include "Monster.h"
class CLeech :
	public CMonster
{
public:
	CLeech();
	~CLeech();

	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;

private:
	virtual void Release() override;
	// CMonster을(를) 통해 상속됨
	virtual void Animation(HDC hDC) override;
	virtual void MonsterUpdate() override;

private:
	int aniCount;
	int explosionCount = 0;
};

