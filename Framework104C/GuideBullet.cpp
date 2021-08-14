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

	// A - B => B�� A�� ���ϴ� ����.
	float w = m_pTarget->GetInfo().fX - m_tInfo.fX;
	float h = m_pTarget->GetInfo().fY - m_tInfo.fY;
	float d = sqrtf(w * w + h * h);

	// ���� ����.
	m_fAngle = acosf(w / d);

	// acosf �Լ��� ���� ���ΰ��� ������ 0 ~ 180�� �̱⿡ ������ ���� ����ó��!
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
