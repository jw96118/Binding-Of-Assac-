#include "stdafx.h"
#include "MonstroBullet_1.h"


CMonstroBullet_1::CMonstroBullet_1()
{
}


CMonstroBullet_1::~CMonstroBullet_1()
{
}

void CMonstroBullet_1::Initialize()
{
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	
	firstX = m_tInfo.fX;
	firstY = m_tInfo.fY;

	CImageMgr::GetInstance()->AddImage(L"MONSTER_TEAR", L"Image/effect/effect_003_bloodtear.png", 256, 256);

	hMemDC = CImageMgr::GetInstance()->GetDc(L"MONSTER_TEAR");


	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;
	AnimationCount = 0;
	aniWidth = 0;
	aniHeight = 0;
	aniFlag = false;
	imageSize.x = 62;
	imageSize.y = 62;
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

int CMonstroBullet_1::Update()
{
	
	float w2 = firstX - m_tInfo.fX;
	float h2 = firstY - m_tInfo.fY;
	float d2 = sqrtf(w * w + h * h);
	CObjectMgr::GetInstance()->AddLayer(LAYER_OBJECT, this);

	m_fSpeed =range/50;
	CGameObject::UpdateRect();
	if (m_bIsDead)
		return DEAD_OBJ;
	if (range <= d2)
	{
		aniFlag = true;
	}
	else if (range - 20 <= d2 && !aniFlag)
	{
		
		m_tInfo.fY += 5.f;
	}
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

void CMonstroBullet_1::Render(HDC hDC)
{
	CGameObject::UpdateRect();


	//g.DrawImage(image, Rect((int)m_tInfo.fX - (int)((imageSize.x *1.5) / 2) + (int)g_fScrollX, (int)m_tInfo.fY - (int)((imageSize.y *1.5) / 2) + (int)g_fScrollY, (int)(imageSize.x * 1.5), (int)(imageSize.y * 1.5)), aniWidth, aniHeight, imageSize.x, imageSize.y, UnitPixel);
	GdiTransparentBlt(hDC, (int)m_tInfo.fX - (int)((imageSize.x *1.5) / 2) + (int)g_fScrollX, (int)m_tInfo.fY - (int)((imageSize.y *1.5) / 2) + (int)g_fScrollY, (int)(imageSize.x * 1.5), (int)(imageSize.y * 1.5),
		hMemDC, aniWidth, aniHeight, imageSize.x, imageSize.y, RGB(0, 0, 0));

	if (aniFlag)
	{
		dwCurTime_Update = GetTickCount();
		if (dwCurTime_Update - dwOldTime_Update >= 100)
		{
			AnimationCount++;
			aniWidth += imageSize.x;
			dwOldTime_Update = dwCurTime_Update;
		}
		if (AnimationCount % 4 == 0)
		{
			aniWidth = 0;
			aniHeight += imageSize.y;
		}
	}
}

void CMonstroBullet_1::Release()
{
}
