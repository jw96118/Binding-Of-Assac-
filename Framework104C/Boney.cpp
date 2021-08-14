#include "stdafx.h"
#include "Boney.h"
#include "BonyBullet.h"

CBoney::CBoney()
{
}


CBoney::~CBoney()
{
	Release();
}

void CBoney::Initialize()
{
	state = MONSTER_IDLE;
	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;
	bodyAnimationCount = 0;
	CImageMgr::GetInstance()->AddImage(L"BONEY_HEAD", L"Image/monster/monster_227_boney head.png",96,32);
	CImageMgr::GetInstance()->AddImage(L"BONEY_BODY", L"Image/monster/monster_227_boney body.png",256,128);
	CImageMgr::GetInstance()->AddImage(L"BONEY_HEAD_2", L"Image/monster/monster_227_boney head_2.png", 96, 32);
	CImageMgr::GetInstance()->AddImage(L"BONEY_BODY_2", L"Image/monster/monster_227_boney body_2.png", 256, 128);
	imageSize.x = 80;
	imageSize.y = 80;

	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	shotSpeed = 30.f;
	range = 1000.f;

	m_fSpeed = 3.5f;
	type = MONSTER;
	hp = 10.f;
	shotCount = 0;
	srand((unsigned)time(NULL));
	deadX = 0;
	deadY = 0;
	deadCount = 0;
	
}

int CBoney::Update()
{
	
	if (m_bIsDead)
		return DEAD_OBJ;
	m_pTarget = CObjectMgr::GetInstance()->GetPlayer();
	
	if (deadCount > 10)
	{
		return DEAD_OBJ;
	}
	CGameObject::UpdateRect();
	if (hp <= 0)
	{
		m_tInfo.fCX = 0.f;
		m_tInfo.fCY = 0.f;

		state = MONSTER_DEAD;
	}
	if (state != MONSTER_DEAD)
		MonsterUpdate();
	CObjectMgr::GetInstance()->AddLayer(LAYER_OBJECT, this);

	return NO_EVENT;
}

void CBoney::Render(HDC hDC)
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

			GdiTransparentBlt(tempDc2, SrenderX, SrenderY + 18, SHADOWX / 2, SHADOWY / 2,
				hMemDC, 0, 0, SHADOWX, SHADOWY, RGB(255, 255, 255));

			GdiAlphaBlend(hDC, SrenderX, SrenderY + 18, SHADOWX / 2, SHADOWY / 2,
				tempDc2, SrenderX, SrenderY + 18, SHADOWX / 2, SHADOWY / 2, bf);

			DeleteDC(tempDc2);
		}
	}
	Animation(hDC);
}



void CBoney::Release()
{
}

void CBoney::Animation(HDC hDC)
{
	HDC tempDc = CreateCompatibleDC(NULL);
	HBITMAP paper = CreateCompatibleBitmap(hDC, WINCX, WINCY);
	HBITMAP oldPaper = (HBITMAP)SelectObject(tempDc, paper);
	DeleteObject(paper);

	//image = CImageMgr::GetInstance()->GetImage(L"BONEY_BODY");
	if (state != MONSTER_DEAD)
	{
		//몸 애니메이션
		if (state != IDLE)
		{
			if (colBullet)
			{
				hMemDC = CImageMgr::GetInstance()->GetDc(L"BONEY_BODY_2");
			}
			else
			{
				hMemDC = CImageMgr::GetInstance()->GetDc(L"BONEY_BODY");
			}
			if (bodyAnimationCount >= 8)
			{
				if (state == MONSTER_MOVE_TOP || state == MONSTER_MOVE_BOTTOM)
				{
					aniWidth = -8;
					aniHeight = SIZEY;
				}
				else
				{
					aniWidth = -8;
					aniHeight = SIZEY * 3;
				}
			}
			else if (bodyAnimationCount < 8)
			{
				if (state == MONSTER_MOVE_TOP || state == MONSTER_MOVE_BOTTOM)
				{
					aniWidth = 0;
					aniHeight = 0;
				}
				else if (state == MONSTER_MOVE_LEFT || state == MONSTER_MOVE_RIGHT)
				{
					aniWidth = 0;
					aniHeight = SIZEY * 2;
				}
			}
			switch (state)
			{
			case MONSTER_MOVE_LEFT:
			case MONSTER_ATTACK_LEFT:
				//g.DrawImage(image, Rect(renderX + imageSize.x, renderY, -imageSize.x, imageSize.y), SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, UnitPixel);

				StretchBlt(tempDc, SIZEX, 0, -SIZEX, SIZEY, hMemDC, SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, SRCCOPY);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, tempDc, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			case MONSTER_MOVE_RIGHT:
			case MONSTER_ATTACK_RIGHT:
				//g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, UnitPixel);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
					hMemDC, SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, RGB(0, 0, 0));

				break;
			case MONSTER_MOVE_TOP:
			case MONSTER_ATTACK_TOP:
			case MONSTER_MOVE_BOTTOM:
			case MONSTER_ATTACK_BOTTOM:
				//g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, UnitPixel);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
					hMemDC, SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, RGB(0, 0, 0));

				break;
			}
		}
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 150)
		{
			bodyAnimationCount++;
			if (bodyAnimationCount > 9)
			{
				bodyAnimationCount = 0;
			}
			dwOldTime_Render = dwCurTime_Render;
		}
		switch (state)
		{
		case IDLE:
			//g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), 0, SIZEY, SIZEX, SIZEY, UnitPixel);
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, 0, SIZEY, SIZEX, SIZEY, RGB(0, 0, 0));

			bodyAnimationCount = 0;
			aniWidth = 0;
			aniHeight = 0;
		}
		//머리 애니메이션
		//image = CImageMgr::GetInstance()->GetImage(L"BONEY_HEAD");
		if (colBullet)
		{
			hMemDC = CImageMgr::GetInstance()->GetDc(L"BONEY_HEAD_2");
			colBullet = false;
		}
		else
		    hMemDC = CImageMgr::GetInstance()->GetDc(L"BONEY_HEAD");
		switch (state)
		{
		case IDLE:
			//g.DrawImage(image, Rect(renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y), 0, 0, SIZEX, SIZEY, UnitPixel);	
			GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
				hMemDC, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_MOVE_LEFT:
		case MONSTER_ATTACK_LEFT:
			StretchBlt(tempDc, SIZEX, 0, -SIZEX, SIZEY, hMemDC, SIZEX, 0, SIZEX, SIZEY, SRCCOPY);
			GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y, tempDc, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));

			//g.DrawImage(image, Rect(renderX + imageSize.x, renderY - (int)m_tInfo.fCY + 15, -imageSize.x, imageSize.y), SIZEX, 0, SIZEX, SIZEY, UnitPixel);

			break;
		case MONSTER_MOVE_RIGHT:
		case MONSTER_ATTACK_RIGHT:
			//g.DrawImage(image, Rect(renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y), SIZEX, 0, SIZEX, SIZEY, UnitPixel);
			GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
				hMemDC, SIZEX, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_MOVE_TOP:
		case MONSTER_ATTACK_TOP:
			//	g.DrawImage(image, Rect(renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y), SIZEX * 2, 0, SIZEX, SIZEY, UnitPixel);
			GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
				hMemDC, SIZEX * 2, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_MOVE_BOTTOM:
		case MONSTER_ATTACK_BOTTOM:
			//g.DrawImage(image, Rect(renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y), 0, 0, SIZEX, SIZEY, UnitPixel);
			GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
				hMemDC, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		}
	
	}
	else
	{
	    hMemDC = CImageMgr::GetInstance()->GetDc(L"MONSTER_DEAD");
		CGameObject::PlayerSound(L"meaty deaths 3.wav", SOUNDMGR::MONSTER_DEAD);
		m_tInfo.fCX = 0;
		m_tInfo.fCY = 0;
		CGameObject::UpdateRect();
	    GdiTransparentBlt(hDC, renderX, renderY, 100, 100,hMemDC, DEADIMAGEX*deadX, DEADIMAGEY*deadY, DEADIMAGEX, DEADIMAGEY, RGB(0, 0, 0));
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 100)
		{
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

void CBoney::MonsterUpdate()
{
	float makeShotPosX = m_tInfo.fX;
	float makeShotPosY = m_tInfo.fY - m_tInfo.fCY + 10;

	dwCurTime_Update = GetTickCount();
	if (dwCurTime_Update - dwOldTime_Update >= 250)
	{
		state = (MONSTER_STATE)(rand() % 4 + 1);
		dwOldTime_Update = dwCurTime_Update;
	}
	if ((m_pTarget->GetInfo().fX <= m_tRect.right && m_pTarget->GetInfo().fX >= m_tRect.left)
		|| (m_pTarget->GetInfo().fY <= m_tRect.bottom && m_pTarget->GetInfo().fY >= m_tRect.top))
	{
		if (shotCount % 100 == 0 || shotCount == 0)
		{
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
			else if((m_pTarget->GetInfo().fY <= m_tRect.bottom && m_pTarget->GetInfo().fY >= m_tRect.top))
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
			CObjectMgr::GetInstance()->AddBoneyBullet(BONY_BULLET ,makeShotPosX, makeShotPosY, (int)m_pTarget->GetInfo().fX, (int)m_pTarget->GetInfo().fY, range);
			CSoundMgr::GetInstance()->PlaySound(L"scamper 1.wav", SOUNDMGR::MONSTER);
		}
		shotCount++;
	}
	else
	{
		shotCount = 0;
		switch (state)
		{
		case MONSTER_IDLE:
			break;
		case MONSTER_MOVE_LEFT:
			m_tInfo.fX -= m_fSpeed;
			break;
		case MONSTER_MOVE_RIGHT:
			m_tInfo.fX += m_fSpeed;
			break;
		case MONSTER_MOVE_TOP:
			m_tInfo.fY -= m_fSpeed;
			break;
		case MONSTER_MOVE_BOTTOM:
			m_tInfo.fY += m_fSpeed;
			break;
		case MONSTER_DEAD:
			break;
		}
	}
}
