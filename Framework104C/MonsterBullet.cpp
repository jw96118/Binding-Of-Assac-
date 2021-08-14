#include "stdafx.h"
#include "MonsterBullet.h"


CMonsterBullet::CMonsterBullet()
{
}


CMonsterBullet::~CMonsterBullet()
{
}

void CMonsterBullet::Initialize()
{
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	m_fSpeed = 10.f;
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
}

int CMonsterBullet::Update()
{
	CGameObject::UpdateRect();
	if (m_bIsDead)
		return DEAD_OBJ;
	CObjectMgr::GetInstance()->AddLayer(LAYER_OBJECT, this);

	if (moveX == 1)
	{
		if (m_tInfo.fX - firstX >= range)
		{
			aniFlag = true;
		}
		else if (m_tInfo.fX - firstX >= range - 100.f)
		{
			if (!aniFlag)
				m_tInfo.fY += 2.f;
		}
	}
	else if (moveX == -1)
	{
		if (m_tInfo.fX - firstX <= -range)
		{
			aniFlag = true;
		}
		else if (m_tInfo.fX - firstX <= -range + 100.f)
		{
			if (!aniFlag)
				m_tInfo.fY += 2.f;
		}
	}
	if (!aniFlag)
	{
		m_tInfo.fX += moveX * m_fSpeed;
		m_tInfo.fY += moveY * m_fSpeed;
	}
	else
	{
		m_tInfo.fCX = 0.f;
		m_tInfo.fCY = 0.f;
	}
	if (AnimationCount > 15)
	{
		//cout << "»èÁ¦" << endl;
		return DEAD_OBJ;

	}
	return NO_EVENT;
}

void CMonsterBullet::Render(HDC hDC)
{
	CGameObject::UpdateRect();


	//g.DrawImage(image, Rect((int)m_tInfo.fX - (int)((imageSize.x *1.5) / 2) + (int)g_fScrollX, (int)m_tInfo.fY - (int)((imageSize.y *1.5) / 2) + (int)g_fScrollY, (int)(imageSize.x * 1.5), (int)(imageSize.y * 1.5)), aniWidth, aniHeight, imageSize.x, imageSize.y, UnitPixel);
	GdiTransparentBlt(hDC, (int)m_tInfo.fX - (int)((imageSize.x *1.5) / 2) + (int)g_fScrollX, (int)m_tInfo.fY - (int)((imageSize.y *1.5) / 2) + (int)g_fScrollY, (int)(imageSize.x * 1.5), (int)(imageSize.y * 1.5),
		hMemDC, aniWidth, aniHeight, imageSize.x, imageSize.y, RGB(0, 0, 0));

	if (aniFlag)
	{
		CGameObject::PlayerSound(L"tear fire 4.wav", SOUNDMGR::EFFECT);
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

void CMonsterBullet::Release()
{
}
