#include "stdafx.h"
#include "Monster.h"


CMonster::CMonster()
	: m_pTarget(nullptr)
{
}


CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 100.f;

	m_fSpeed = 3.f;
	type = MONSTER;
}

int CMonster::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;

	// 과제2. 플레이어 따라가기. (역삼각함수 acosf를 이용하여 각도 구하기)
	m_pTarget = CObjectMgr::GetInstance()->GetPlayer();
	NULL_CHECK_RETURN(m_pTarget, NO_EVENT);

	//// A - B => B가 A로 향하는 방향.
	//float w = m_pTarget->GetInfo().fX - m_tInfo.fX;
	//float h = m_pTarget->GetInfo().fY - m_tInfo.fY;
	//float d = sqrtf(w * w + h * h);

	//// 라디안 단위.
	//m_fAngle = acosf(w / d);

	//// acosf 함수가 구한 끼인각의 범위는 0 ~ 180도 이기에 다음과 같은 예외처리!
	//if (m_tInfo.fY < m_pTarget->GetInfo().fY)
	//	m_fAngle *= -1.f;

	//m_tInfo.fX += cosf(m_fAngle) * m_fSpeed;
	//m_tInfo.fY -= sinf(m_fAngle) * m_fSpeed;

	return NO_EVENT;
}

void CMonster::Render(HDC hDC)
{
	CGameObject::UpdateRect();
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CMonster::Release()
{
}

void CMonster::SetMonsterType(MONSTER_TYPE _monster_Type)
{
	monster_Type = _monster_Type;
}

void CMonster::SetHp(float _hp)
{
	hp -= _hp;
}

void CMonster::SetColBullet(bool flag)
{
	colBullet = flag;
}

MONSTER_STATE CMonster::GetState()
{
	return state;
}

void CMonster::Refresh()
{
	srand((unsigned)time(NULL));
	state = (MONSTER_STATE)(rand() % 4 + 1);
}

void CMonster::SetMonsterSRectP(int x, int y)
{
	monsterShieldP.x = x;
	monsterShieldP.y = y;
}

const bool & CMonster::GetUpdateStartCheck() const
{
	return UpdateStartCheck;
}

const RECT& CMonster::GetMonsterSRect() const
{
	return monsterShield;
}

const POINT & CMonster::GetMonsterSRectP() const
{
	return monsterShieldP;
}

const float & CMonster::GetMonsterHP() const
{
	return hp;
}

const MONSTER_TYPE & CMonster::GetMonsterType() const
{
	return monster_Type;
}

void CMonster::UpdateShieldRect()
{
	monsterShield.left = LONG(monsterShieldP.x - m_tInfo.fCX * 0.5f);
	monsterShield.top = LONG(monsterShieldP.y - m_tInfo.fCY * 0.5f);
	monsterShield.right = LONG(monsterShieldP.x + m_tInfo.fCX * 0.5f);
	monsterShield.bottom = LONG(monsterShieldP.y + m_tInfo.fCY * 0.5f);
}
