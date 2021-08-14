#include "stdafx.h"
#include "GuideBullet.h"


CGuideBullet::CGuideBullet()
	: m_pTarget(nullptr)
{
}


CGuideBullet::~CGuideBullet()
{
	Release();
}

void CGuideBullet::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	m_fSpeed = 10.f;
}

int CGuideBullet::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;

	m_pTarget = CObjectMgr::GetInstance()->GetNearTarget(this, MONSTER);
	NULL_CHECK_RETURN(m_pTarget, DEAD_OBJ);
	CObjectMgr::GetInstance()->AddLayer(LAYER_OBJECT, this);

	// A - B => B가 A로 향하는 방향.
	float w = m_pTarget->GetInfo().fX - m_tInfo.fX;
	float h = m_pTarget->GetInfo().fY - m_tInfo.fY;
	float d = sqrtf(w * w + h * h);

	// 라디안 단위.
	m_fAngle = acosf(w / d);

	// acosf 함수가 구한 끼인각의 범위는 0 ~ 180도 이기에 다음과 같은 예외처리!
	if (m_tInfo.fY < m_pTarget->GetInfo().fY)
		m_fAngle *= -1.f;

	m_tInfo.fX += cosf(m_fAngle) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle) * m_fSpeed;

	return NO_EVENT;
}

void CGuideBullet::Render(HDC hDC)
{
	CGameObject::UpdateRect();
	Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CGuideBullet::Release()
{
}
