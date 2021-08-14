#include "stdafx.h"
#include "PlayerBullet.h"

#define BULLETX 62
#define BULLETY 62
CPlayerBullet::CPlayerBullet()
{
}


CPlayerBullet::~CPlayerBullet()
{
}

void CPlayerBullet::Initialize()
{

	m_fSpeed = 10.f;
	firstX = m_tInfo.fX;
	firstY = m_tInfo.fY;

	CImageMgr::GetInstance()->AddImage(L"TEAR1", L"Image/effect/effect_015_tearpoofa.png", 256, 256);
	CImageMgr::GetInstance()->AddImage(L"PACT_TEAR", L"Image/effect/effect_003_bloodtear.png",256,256);

	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;
	AnimationCount = 0;
	aniWidth = 0;
	aniHeight = 0;
	aniFlag = false;
	testbool = false;
	shadowY = 0;
	type = PLAYER_BULLET;
}

int CPlayerBullet::Update()
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
			{
				m_tInfo.fY += 2.f;
				shadowY -= 2;
			}
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
			{
				m_tInfo.fY += 2.f;
				shadowY -= 2;
			}
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
		//cout << "삭제" << endl;
		return DEAD_OBJ;

	}
	return NO_EVENT;
}

void CPlayerBullet::Render(HDC hDC)
{
	CGameObject::UpdateRect();


	int SrenderX = (int)m_tInfo.fX - SHADOWX / 4 + (int)g_fScrollX;
	int SrenderY = (int)m_tInfo.fY - SHADOWY / 4 + (int)g_fScrollY + shadowY;

	if (!aniFlag)
	{
		BLENDFUNCTION bf;
		bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
		bf.BlendFlags = 0; // 무조건 0이어야 한다
		bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미
		bf.SourceConstantAlpha = 127; // 투명도(투명 0 - 불투명 255)
		if (!shadow)
		{
			HDC tempDc2 = CreateCompatibleDC(NULL);
			HBITMAP paper2 = CreateCompatibleBitmap(hDC, WINCX, WINCY);
			HBITMAP oldPaper2 = (HBITMAP)SelectObject(tempDc2, paper2);
			DeleteObject(paper2);

			hMemDC = CImageMgr::GetInstance()->GetDc(L"ROOM_RENDER");

			GdiTransparentBlt(tempDc2, 0, 0, WINCX, WINCY,
				hMemDC, 0, 0, WINCX, WINCY, RGB(255, 255, 255));

			hMemDC = CImageMgr::GetInstance()->GetDc(L"SHADOW");

			GdiTransparentBlt(tempDc2, SrenderX, SrenderY + 50, SHADOWX / 4, SHADOWY / 4,
				hMemDC, 0, 0, SHADOWX, SHADOWY, RGB(255, 255, 255));

			GdiAlphaBlend(hDC, SrenderX, SrenderY + 50, SHADOWX / 4, SHADOWY / 4,
				tempDc2, SrenderX, SrenderY + 50, SHADOWX / 4, SHADOWY / 4, bf);

			DeleteDC(tempDc2);
		}
	}


	hMemDC = CImageMgr::GetInstance()->GetDc(bullet_type);
	//g.DrawImage(image, Rect((int)m_tInfo.fX - (int)((imageSize.x *1.5) / 2) + (int)g_fScrollX, (int)m_tInfo.fY - (int)((imageSize.y *1.5) / 2) + (int)g_fScrollY, (int)(imageSize.x * 1.5), (int)(imageSize.y * 1.5)), aniWidth, aniHeight, imageSize.x, imageSize.y, UnitPixel);
	GdiTransparentBlt(hDC, (int)m_tInfo.fX - (int)((imageSize.x *1.5) / 2) + (int)g_fScrollX, (int)m_tInfo.fY - (int)((imageSize.y *1.5) / 2) + (int)g_fScrollY, (int)(imageSize.x * 1.5), (int)(imageSize.y * 1.5),
		hMemDC, aniWidth, aniHeight, BULLETX, BULLETY, RGB(0, 0, 0));

	if (aniFlag)
	{
		//CGameObject::PlayerSound(L"tear fire 4.wav", SOUNDMGR::EFFECT);
		if (!testbool)
		{
			CSoundMgr::GetInstance()->PlaySound(L"tear fire 4.wav", SOUNDMGR::EFFECT);
			testbool = true;
		}
		dwCurTime_Update = GetTickCount();
		if (dwCurTime_Update - dwOldTime_Update >= 100)
		{
			AnimationCount++;
			aniWidth += BULLETX;
			dwOldTime_Update = dwCurTime_Update;
		}
		if (AnimationCount % 4 == 0)
		{
			aniWidth = 0;
			aniHeight += BULLETY;
		}
	}
}

void CPlayerBullet::Release()
{
}

void CPlayerBullet::SetBulletType(wstring _bullet_type)
{
	bullet_type = _bullet_type;
}

void CPlayerBullet::SetImageSize(POINT pt)
{
	imageSize = pt;
}

void CPlayerBullet::SetColSize(float x, float y)
{
	m_tInfo.fCX = x;
	m_tInfo.fCY = y;
}
