#pragma once
#include "Monster.h"
class CMaskAndHeart :
	public CMonster
{
public:
	CMaskAndHeart();
	~CMaskAndHeart();
	// CGameObject��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;

private:
	virtual void Release() override;
	// CMonster��(��) ���� ��ӵ�
	virtual void Animation(HDC hDC) override;
	virtual void MonsterUpdate() override;

private:
	int renderMaskX;
	int renderMaskY;
	int count = 0;
	bool attackflag;
};

