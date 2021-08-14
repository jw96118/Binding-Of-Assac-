#pragma once
#include "Monster.h"
class CMonstro :
	public CMonster
{
public:
	CMonstro();
	~CMonstro();
public:
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
	void jumpMove();
	void MakeBullet_1();
	void MakeBullet_2();
private:
	MONSTRO_PATTERN pattern;
	MONSTRO_PATTERN prePattern;
	int aniCountX;
	int aniCountY;
	POINT blood;
	RECT deadBlood;
	RECT deadBloodP;
	int deadBloodCount;
	int deadCount1;
	int bloodCount;
	bool nowMove;
	bool nowJump;
	bool longJump;
	bool nowShot;
	bool bulletMake;
	float w;
	float h;
	float d;
	INFO preTarget;
	int shadowY;

	int sSizeX;
	int sSizeY;
};

