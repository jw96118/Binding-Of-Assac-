#pragma once
#include "Monster.h"
class CBoney :
	public CMonster
{
public:
	CBoney();
	~CBoney();
public:
	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;

private:
	virtual void Release() override;
	// CMonster을(를) 통해 상속됨
	virtual void Animation(HDC hDC) override;
	virtual void MonsterUpdate() override;

private:
	unsigned int bodyAnimationCount;
	
	int shotCount;
};

