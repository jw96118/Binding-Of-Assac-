#include "stdafx.h"
#include "Shield.h"


CShield::CShield()
	: m_fDistance(0.f),
	m_pTarget(nullptr)
{
}


CShield::~CShield()
{
	Release();
}

void CShield::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	//m_fAngle = 90.f;
	m_fSpeed = 5.f;
	m_fDistance = 40.f;

	CImageMgr::GetInstance()->AddImage(L"SHIELD", L"Image/item/monster_010_fly.png", 256, 256);
	hMemDC = CImageMgr::GetInstance()->GetDc(L"SHIELD");
	imageSize.x = 80;
	imageSize.y = 80;
	count = 0;
	type = SHIELD;
	layer = LAYER_OBJECT;
}

int CShield::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;

	m_fAngle += m_fSpeed;

	m_pTarget = CObjectMgr::GetInstance()->GetPlayer();
	NULL_CHECK_RETURN(m_pTarget, DEAD_OBJ);

	m_tInfo.fX = m_pTarget->GetInfo().fX + cosf(m_fAngle * PI / 180.f) * m_fDistance;
	m_tInfo.fY = m_pTarget->GetInfo().fY - sinf(m_fAngle * PI / 180.f) * m_fDistance;

	CObjectMgr::GetInstance()->AddLayer(layer, this);
	return NO_EVENT;
}

void CShield::Render(HDC hDC)
{
	CGameObject::UpdateRect();
	int renderX = (int)m_tInfo.fX - imageSize.x / 2 + (int)g_fScrollX;
	int renderY = (int)m_tInfo.fY - imageSize.y / 2 + (int)g_fScrollY;
	int x = 0;
	dwCurTime_Render = GetTickCount();
	if (dwCurTime_Render - dwOldTime_Render >= 60)
	{
		if (count % 2 == 0 )
		{
			x = 0;
		}
		else
		{
			x = 1;
		}
		count++;
		dwOldTime_Render = dwCurTime_Render;
	}

	GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, SIZEX * x, 0, SIZEX, SIZEY, RGB(0, 0, 0));

}

void CShield::Release()
{
}

void CShield::SetLayer(RENDER_LAYER _layer)
{
	layer = _layer;
}
