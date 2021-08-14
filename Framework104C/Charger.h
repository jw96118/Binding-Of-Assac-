#pragma once
#include "Monster.h"
class CCharger :
	public CMonster
{
public:
	CCharger();
	~CCharger();
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;

private:
	virtual void Release() override;
	// CMonster��(��) ���� ��ӵ�
	virtual void Animation(HDC hDC) override;
	virtual void MonsterUpdate() override;
private:
	int aniCount;
};

