#include "stdafx.h"
#include "BonyBullet.h"


CBonyBullet::CBonyBullet()
{
}


CBonyBullet::~CBonyBullet()
{
}

void CBonyBullet::Initialize()
{
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	m_fSpeed = 10.f;
	firstX = m_tInfo.fX;
	firstY = m_tInfo.fY;

	CImageMgr::GetInstance()->AddImage(L"TEAR_BONE", L"Image/effect/bone_projectile.png", 128, 32);
	CImageMgr::GetInstance()->AddImage(L"TEAR_BONE_BREAK", L"Image/effect/effect_033_toothgibs.png",64,32);

	//image = CImageMgr::GetInstance()->GetImage(L"TEAR_BONE");
	hMemDC = CImageMgr::GetInstance()->GetDc(L"TEAR_BONE");

	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;
	AnimationCount = 0;
	aniWidth = 0;
	aniHeight = 0;
	aniFlag = false;
	imageSize.x = 32;
	imageSize.y = 32;
	type = MONSTER_BULLET;
	breakCount = 0;

	 w = moveX - m_tInfo.fX;
	 h = moveY - m_tInfo.fY;
	 d = sqrtf(w * w + h * h);

	// 라디안 단위.
	m_fAngle = acosf(w / d);

	// acosf 함수가 구한 끼인각의 범위는 0 ~ 180도 이기에 다음과 같은 예외처리!
	if (m_tInfo.fY < moveY)
		m_fAngle *= -1.f;
}

int CBonyBullet::Update()
{
	CGameObject::UpdateRect();
	if (m_bIsDead)
		return DEAD_OBJ;
	CObjectMgr::GetInstance()->AddLayer(LAYER_OBJECT, this);

	if (aniFlag)
	{
		m_tInfo.fCX = 0.f;
		m_tInfo.fCY = 0.f;
	}
	else
	{
		m_tInfo.fX += cosf(m_fAngle) * m_fSpeed;
		m_tInfo.fY -= sinf(m_fAngle) * m_fSpeed;
	}
	if (breakCount > 2)
	{
		imageSize.x = 0;
		imageSize.y = 0;
		return DEAD_OBJ;
	}
	return NO_EVENT;
}

void CBonyBullet::Render(HDC hDC)
{
	CGameObject::UpdateRect();
    //	Graphics g(hDC);
	//g.DrawImage(image, Rect((int)m_tInfo.fX - (int)((imageSize.x *1.5) / 2) + (int)g_fScrollX, (int)m_tInfo.fY - (int)((imageSize.y *1.5) / 2) + (int)g_fScrollY, imageSize.x * 1.5, imageSize.y * 1.5), imageSize.x *AnimationCount, 0, imageSize.x, imageSize.y, UnitPixel);
	GdiTransparentBlt(hDC, (int)m_tInfo.fX - (int)((imageSize.x *1.5) / 2) + (int)g_fScrollX, (int)m_tInfo.fY - (int)((imageSize.y *1.5) / 2) + (int)g_fScrollY, imageSize.x * 1.5, imageSize.y * 1.5,
		hMemDC, imageSize.x *AnimationCount, 0, imageSize.x, imageSize.y, RGB(0, 0, 0));
	if (!aniFlag)
	{
		dwCurTime_Update = GetTickCount();
		if (dwCurTime_Update - dwOldTime_Update >= 60)
		{
			AnimationCount++;
			dwOldTime_Update = dwCurTime_Update;
		}
		if (AnimationCount >= 4)
		{
			AnimationCount = 0;
		}
	}
	else
	{
		hMemDC = CImageMgr::GetInstance()->GetDc(L"TEAR_BONE_BREAK");
		GdiTransparentBlt(hDC, (int)m_tInfo.fX - 32 + (int)g_fScrollX, (int)m_tInfo.fY - 32 + (int)g_fScrollY, imageSize.x, imageSize.y,
			hMemDC, 32* breakCount, 0, imageSize.x, imageSize.y, RGB(0, 0, 0));

		dwCurTime_Update = GetTickCount();
		if (dwCurTime_Update - dwOldTime_Update >= 60)
		{
			breakCount++;
			dwOldTime_Update = dwCurTime_Update;
		}
	
	}
}

void CBonyBullet::Release()
{
}
