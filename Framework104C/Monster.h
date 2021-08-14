#pragma once
#include "GameObject.h"
class CMonster :
	public CGameObject
{
public:
	CMonster();
	virtual ~CMonster();

public:
	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;

protected:
	virtual void Release() override;
	virtual void Animation(HDC hDC) = 0;
	virtual void MonsterUpdate() = 0;

public:
	void SetMonsterType(MONSTER_TYPE    monster_Type);
	void SetHp(float _hp);
	void SetColBullet(bool flag);
	MONSTER_STATE GetState();
	void Refresh();
	void SetMonsterSRectP(int x, int y);
	const bool& GetUpdateStartCheck() const;
	const RECT& GetMonsterSRect() const;
	const POINT& GetMonsterSRectP() const;
	const float& GetMonsterHP() const;
	const MONSTER_TYPE& GetMonsterType() const;
protected:
	void UpdateShieldRect();
protected:
	CGameObject*	m_pTarget;
	MONSTER_TYPE    monster_Type;
	MONSTER_STATE   state; 
	RECT  monsterShield;
	POINT monsterShieldP;
	float moveSpeed;
	float shotSpeed;
	float atk;
	float bulletSpeed;
	float hp;
	float range;
	int deadX;
	int deadY;
	int deadCount;
	int renderX;
	int renderY;
	bool UpdateStartCheck;
	bool colBullet;
};

