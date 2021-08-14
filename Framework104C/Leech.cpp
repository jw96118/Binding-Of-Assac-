#include "stdafx.h"
#include "Leech.h"


CLeech::CLeech()
{
}


CLeech::~CLeech()
{
}

void CLeech::Initialize()
{
	state = MONSTER_IDLE;
	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;

	CImageMgr::GetInstance()->AddImage(L"LEECH", L"Image/monster/monster_172_leech.png", 192, 96);
	CImageMgr::GetInstance()->AddImage(L"LEECH_2", L"Image/monster/monster_172_leech_2.png", 192, 96);
	imageSize.x = 80;
	imageSize.y = 80;

	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	shotSpeed = 30.f;
	range = 1000.f;

	m_fSpeed = 6.f;
	type = MONSTER;
	hp = 10.f;

	srand((unsigned)time(NULL));
	deadX = 0;
	deadY = 0;
	deadCount = 0;

	aniCount = 0;

	state = (MONSTER_STATE)(rand() % 4 + 1);
}

int CLeech::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;
	m_pTarget = CObjectMgr::GetInstance()->GetPlayer();
	
	if (deadCount > 12)
	{
		return DEAD_OBJ;
	}
	CGameObject::UpdateRect();
	if (hp <= 0)
	{
		if (explosionCount == 0)
		{
			m_tInfo.fCX = 300.f;
			m_tInfo.fCY = 300.f;
		}
		else
		{
			m_tInfo.fCX = 0.f;
			m_tInfo.fCY = 0.f;
		}
		type = EXPLOSION;
		explosionCount++;
		state = MONSTER_DEAD; 
		imageSize.x = 96*2;
		imageSize.y = 96*2;
		
	}
	if (state != MONSTER_DEAD)
		MonsterUpdate();
	CObjectMgr::GetInstance()->AddLayer(LAYER_FLY_OBJECT, this);

	return NO_EVENT;
}

void CLeech::Render(HDC hDC)
{
	CGameObject::UpdateRect();
	renderX = (int)m_tInfo.fX - imageSize.x / 2 + (int)g_fScrollX;
	renderY = (int)m_tInfo.fY - imageSize.y / 2 + (int)g_fScrollY;

	int SrenderX = (int)m_tInfo.fX - SHADOWX / 4 + (int)g_fScrollX;
	int SrenderY = (int)m_tInfo.fY - SHADOWY / 4 + (int)g_fScrollY;


	BLENDFUNCTION bf;
	bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
	bf.BlendFlags = 0; // 무조건 0이어야 한다
	bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미
	bf.SourceConstantAlpha = 127; // 투명도(투명 0 - 불투명 255)
	if (!shadow)
	{
		if (state != MONSTER_DEAD)
		{
			HDC tempDc2 = CreateCompatibleDC(NULL);
			HBITMAP paper2 = CreateCompatibleBitmap(hDC, WINCX, WINCY);
			HBITMAP oldPaper2 = (HBITMAP)SelectObject(tempDc2, paper2);
			DeleteObject(paper2);

			hMemDC = CImageMgr::GetInstance()->GetDc(L"ROOM_RENDER");

			GdiTransparentBlt(tempDc2, 0, 0, WINCX, WINCY,
				hMemDC, 0, 0, WINCX, WINCY, RGB(255, 255, 255));

			hMemDC = CImageMgr::GetInstance()->GetDc(L"SHADOW");

			GdiTransparentBlt(tempDc2, SrenderX, SrenderY + 30, SHADOWX / 2, SHADOWY / 2,
				hMemDC, 0, 0, SHADOWX, SHADOWY, RGB(255, 255, 255));

			GdiAlphaBlend(hDC, SrenderX, SrenderY + 30, SHADOWX / 2, SHADOWY / 2,
				tempDc2, SrenderX, SrenderY + 30, SHADOWX / 2, SHADOWY / 2, bf);

			DeleteDC(tempDc2);
		}
	}
	hMemDC = CImageMgr::GetInstance()->GetDc(L"LEECH");
	Animation(hDC);
}

void CLeech::Release()
{
}

void CLeech::Animation(HDC hDC)
{

	HDC tempDc = CreateCompatibleDC(NULL);
	HBITMAP paper = CreateCompatibleBitmap(hDC, WINCX, WINCY);
	HBITMAP oldPaper = (HBITMAP)SelectObject(tempDc, paper);
	DeleteObject(paper);
	if (colBullet)
	{
		colBullet = false;
		hMemDC = CImageMgr::GetInstance()->GetDc(L"LEECH_2");
	}
	else
		hMemDC = CImageMgr::GetInstance()->GetDc(L"LEECH");
	if (state < MONSTER_ATTACK_LEFT)
	{
		
		switch (state)
		{
		case MONSTER_IDLE:
			break;
		case MONSTER_MOVE_LEFT:
			StretchBlt(tempDc, SIZEX, 0, -SIZEX, SIZEY, hMemDC, SIZEX * aniCount, 0, SIZEX, SIZEY, SRCCOPY);
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, tempDc, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_MOVE_RIGHT:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, SIZEX * aniCount, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_MOVE_TOP:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, SIZEX * aniCount, SIZEY, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_MOVE_BOTTOM:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, SIZEX * aniCount, SIZEY * 2, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		}
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 100)
		{
			aniCount++;
			if (aniCount >= 6)
			{
				aniCount = 0;
			}
			dwOldTime_Render = dwCurTime_Render;
		}
	}
	else if (state >= MONSTER_ATTACK_LEFT && state != MONSTER_DEAD)
	{
		switch (state)
		{
		case MONSTER_IDLE:
			break;
		case MONSTER_ATTACK_LEFT:
			StretchBlt(tempDc, SIZEX, 0, -SIZEX, SIZEY, hMemDC, SIZEX * aniCount, 0, SIZEX, SIZEY, SRCCOPY);
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, tempDc, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_ATTACK_RIGHT:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, SIZEX * aniCount, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_ATTACK_TOP:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, SIZEX * aniCount, SIZEY, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_ATTACK_BOTTOM:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, SIZEX * aniCount, SIZEY * 2, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		}
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 100)
		{
			
			aniCount++;
			if (aniCount >= 6)
			{
				aniCount = 0;
			}
			dwOldTime_Render = dwCurTime_Render;
		}
	}
	else if (state == MONSTER_DEAD)
	{
		hMemDC = CImageMgr::GetInstance()->GetDc(L"EXPLOSION");
		CGameObject::PlayerSound(L"boss explosions 0.wav", SOUNDMGR::MONSTER_DEAD);

		CGameObject::UpdateRect();
		GdiTransparentBlt(hDC, renderX, renderY - 96, imageSize.x, imageSize.y , hMemDC, EXPLOSIONIMAGEX*deadX, EXPLOSIONIMAGEY*deadY, EXPLOSIONIMAGEX, EXPLOSIONIMAGEY, RGB(0, 0, 0));
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 100)
		{
			if (deadCount == 0)
			{
				CGameObject::StopBGM(SOUNDMGR::MONSTER);
			}
			deadCount++;
			
			deadX++;
			if (deadX > 4)
			{
				deadX = 0;
				deadY++;
			}
			dwOldTime_Render = dwCurTime_Render;
		}
	}
	DeleteDC(tempDc);
}

void CLeech::MonsterUpdate()
{
	if (state < MONSTER_ATTACK_LEFT)
	{
		m_fSpeed = 6.f;
		if ((m_pTarget->GetInfo().fX <= m_tRect.right && m_pTarget->GetInfo().fX >= m_tRect.left)
			|| (m_pTarget->GetInfo().fY <= m_tRect.bottom && m_pTarget->GetInfo().fY >= m_tRect.top))
		{
			m_fSpeed = 12.f;
			if ((m_pTarget->GetInfo().fX <= m_tRect.right && m_pTarget->GetInfo().fX >= m_tRect.left))
			{
				if (m_pTarget->GetInfo().fY > m_tInfo.fY)
				{
					state = MONSTER_ATTACK_BOTTOM;
				}
				else
				{
					state = MONSTER_ATTACK_TOP;
				}
			}
			else if ((m_pTarget->GetInfo().fY <= m_tRect.bottom && m_pTarget->GetInfo().fY >= m_tRect.top))
			{
				if (m_pTarget->GetInfo().fX > m_tInfo.fX)
				{
					state = MONSTER_ATTACK_RIGHT;
				}
				else
				{
					state = MONSTER_ATTACK_LEFT;
				}
			}
			CGameObject::PlayerSound(L"leech.wav", SOUNDMGR::MONSTER);
		}	
		switch (state)
		{
		case MONSTER_IDLE:
			break;
		case MONSTER_MOVE_LEFT:
			m_tInfo.fX -= m_fSpeed;
			CGameObject::StopBGM( SOUNDMGR::MONSTER);
			break;
		case MONSTER_MOVE_RIGHT:
			CGameObject::StopBGM(SOUNDMGR::MONSTER);
			m_tInfo.fX += m_fSpeed;
			break;
		case MONSTER_MOVE_TOP:
			CGameObject::StopBGM(SOUNDMGR::MONSTER);
			m_tInfo.fY -= m_fSpeed;
			break;
		case MONSTER_MOVE_BOTTOM:
			CGameObject::StopBGM(SOUNDMGR::MONSTER);
			m_tInfo.fY += m_fSpeed;
			break;

		case MONSTER_DEAD:
			break;
		default:
			break;
		}
	}
	else
	{
		switch (state)
		{
		case MONSTER_ATTACK_LEFT:
			m_tInfo.fX -= m_fSpeed;
			break;
		case MONSTER_ATTACK_RIGHT:
			m_tInfo.fX += m_fSpeed;
			break;
		case MONSTER_ATTACK_TOP:
			m_tInfo.fY -= m_fSpeed;
			break;
		case MONSTER_ATTACK_BOTTOM:
			m_tInfo.fY += m_fSpeed;
			break;
		}
	}
}
