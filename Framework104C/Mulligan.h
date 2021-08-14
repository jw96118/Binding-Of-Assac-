#pragma once
#include "Monster.h"
class CMulligan :
	public CMonster
{
public:
	CMulligan();
	~CMulligan();

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
	unsigned int headAnimationCount;
	unsigned int explosionCount;
	unsigned int explosionCount2;
	bool explotion;	
	bool explosionDead;
	bool deadSound = false;
	DWORD dwOldTime_Render2 = GetTickCount();
	DWORD dwCurTime_Render2 = 0;


	int explosionCount_1 = 0;
};

