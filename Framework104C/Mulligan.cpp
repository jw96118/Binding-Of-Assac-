#include "stdafx.h"
#include "Mulligan.h"


CMulligan::CMulligan()
{
}


CMulligan::~CMulligan()
{
}

void CMulligan::Initialize()
{

	state = MONSTER_IDLE;
	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;
	bodyAnimationCount = 0;
	headAnimationCount = 0;
	explosionCount = 0;
	explosionCount2 = 0;
	CImageMgr::GetInstance()->AddImage(L"MULLIGAN_HEAD", L"Image/monster/monster_054_mulligan.png", 288, 216);
	CImageMgr::GetInstance()->AddImage(L"MULLIGAN_HEAD_2", L"Image/monster/monster_054_mulligan_2.png", 288, 216);
	CImageMgr::GetInstance()->AddImage(L"MULLIGAN_BODY", L"Image/monster/monster_000_bodies01.png", 512, 256);
	imageSize.x = 80;
	imageSize.y = 80;

	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	shotSpeed = 30.f;
	range = 1000.f;

	m_fSpeed = 3.5f;
	type = MONSTER;
	hp = 10.f;

	srand((unsigned)time(NULL));
	deadX = 0;
	deadY = 0;
	deadCount = 0;
	explotion = false;
	explosionDead = false;
}

int CMulligan::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;
	m_pTarget = CObjectMgr::GetInstance()->GetPlayer();

	CGameObject::PlayerSound(L"scared whimper 2.wav", SOUNDMGR::MONSTER);

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
	if (explosionDead)
	{
		if (explosionCount_1 == 0)
		{
			m_tInfo.fCX = 300.f;
			m_tInfo.fCY = 300.f;
			CGameObject::StopBGM(SOUNDMGR::MONSTER);
		}
		else
		{
			m_tInfo.fCX = 0.f;
			m_tInfo.fCY = 0.f;
		}
		type = EXPLOSION;
		explosionCount_1++;
	}
	if (state != MONSTER_DEAD)
		MonsterUpdate();
	CObjectMgr::GetInstance()->AddLayer(LAYER_OBJECT, this);

	return NO_EVENT;
}

void CMulligan::Render(HDC hDC)
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

void CMulligan::Release()
{
}

void CMulligan::Animation(HDC hDC)
{
	HDC tempDc = CreateCompatibleDC(NULL);
	HBITMAP paper = CreateCompatibleBitmap(hDC, WINCX, WINCY);
	HBITMAP oldPaper = (HBITMAP)SelectObject(tempDc, paper);
	DeleteObject(paper);

	//image = CImageMgr::GetInstance()->GetImage(L"BONEY_BODY");
	if (state != MONSTER_DEAD)
	{
	switch (state)
	{
	case IDLE:
		//	g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), 0, SIZEY, SIZEX, SIZEY, UnitPixel);
		GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
			hMemDC, 0, SIZEY, SIZEX, SIZEY, RGB(0, 0, 0));
		bodyAnimationCount = 0;
		aniWidth = 0;
		aniHeight = 0;
	}
		//몸 애니메이션
		if (state != MONSTER_IDLE)
		{
			hMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
			if (bodyAnimationCount >= 2)
			{
				if (state == MOVE_UP || state == MOVE_DOWN)
				{
					aniWidth = -2;
					aniHeight = SIZEY;
				}
				else
				{
					if (bodyAnimationCount >= 8)
					{
						aniWidth = -8;
						aniHeight = SIZEY * 3;
					}
				}
			}
			else if (bodyAnimationCount < 2)
			{
				if (state == MOVE_UP || state == MOVE_DOWN)
				{
					aniWidth = 6;
					aniHeight = 0;
				}
				else if (state == MOVE_LEFT || state == MOVE_RIGHT)
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
				//	g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, UnitPixel);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, RGB(0, 0, 0));
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
			hMemDC = CImageMgr::GetInstance()->GetDc(L"MULLIGAN_HEAD_2");
			colBullet = false;
		}
		else
		hMemDC = CImageMgr::GetInstance()->GetDc(L"MULLIGAN_HEAD");
		switch (state)
		{
		case IDLE:
		case MONSTER_MOVE_LEFT:
		case MONSTER_ATTACK_LEFT:
		case MONSTER_MOVE_RIGHT:
		case MONSTER_ATTACK_RIGHT:
		case MONSTER_MOVE_TOP:
		case MONSTER_ATTACK_TOP:
		case MONSTER_MOVE_BOTTOM:
		case MONSTER_ATTACK_BOTTOM:
			if(!explotion)
			//g.DrawImage(image, Rect(renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y), 0, 0, SIZEX, SIZEY, UnitPixel);
			GdiTransparentBlt(hDC, renderX - 20, renderY - 50 , imageSize.x * 1.5, imageSize.y *1.5,
				hMemDC, 48 * headAnimationCount, 0, 48, 48, RGB(0, 0, 0));
			else
			{
				GdiTransparentBlt(hDC, renderX - 20, renderY - 50, imageSize.x * 1.5, imageSize.y *1.5,
					hMemDC, 48 * headAnimationCount, 48, 48, 48, RGB(0, 0, 0));

			}
			break;
		}
		if (!explotion)
		{
			dwCurTime_Render2 = GetTickCount();
			if (dwCurTime_Render2 - dwOldTime_Render2 >= 150)
			{
				headAnimationCount++;
				if (headAnimationCount > 5)
				{
					headAnimationCount = 0;
				}
				dwOldTime_Render2 = dwCurTime_Render2;
			}
		}
		else
		{
			dwCurTime_Render2 = GetTickCount();
			if (dwCurTime_Render2 - dwOldTime_Render2 >= 250)
			{
				if(headAnimationCount>0)
				headAnimationCount--;
				if (headAnimationCount == 0)
				{
					headAnimationCount = 0;
					explosionCount2++;
					cout << explosionCount2 << endl;
					if (explosionCount2 > 2)
					{
						explosionDead = true;
						imageSize.x = 96 * 2;
						imageSize.y = 96 * 2;
						state = MONSTER_DEAD;
					}
				}
				dwOldTime_Render2 = dwCurTime_Render2;
			}
		}
	}
	else
	{
		if (!explosionDead)
		{
		
			hMemDC = CImageMgr::GetInstance()->GetDc(L"MONSTER_DEAD");
			if (!deadSound)
			{
				CSoundMgr::GetInstance()->PlaySound(L"meaty deaths 3.wav", SOUNDMGR::MONSTER_DEAD);
				deadSound = true;
			}
			m_tInfo.fCX = 0;
			m_tInfo.fCY = 0;
			CGameObject::UpdateRect();
			GdiTransparentBlt(hDC, renderX, renderY, 100, 100, hMemDC, DEADIMAGEX*deadX, DEADIMAGEY*deadY, DEADIMAGEX, DEADIMAGEY, RGB(0, 0, 0));
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
		else
		{
			hMemDC = CImageMgr::GetInstance()->GetDc(L"EXPLOSION");
			CGameObject::PlayerSound(L"boss explosions 0.wav", SOUNDMGR::MONSTER_DEAD);

			CGameObject::UpdateRect();
			GdiTransparentBlt(hDC, renderX, renderY - 96, imageSize.x, imageSize.y, hMemDC, EXPLOSIONIMAGEX*deadX, EXPLOSIONIMAGEY*deadY, EXPLOSIONIMAGEX, EXPLOSIONIMAGEY, RGB(0, 0, 0));
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
	}
	DeleteDC(tempDc);
}

void CMulligan::MonsterUpdate()
{
	if (!explotion)
	{
		if (m_pTarget->GetInfo().fX >= m_tInfo.fX)
		{
			state = MONSTER_MOVE_LEFT;
		}
		else if (m_pTarget->GetInfo().fX < m_tInfo.fX)
			state = MONSTER_MOVE_RIGHT;
	}
	float w = m_pTarget->GetInfo().fX - m_tInfo.fX;
	float h = m_pTarget->GetInfo().fY - m_tInfo.fY;
	float d = sqrtf(w * w + h * h);

	// 라디안 단위.
	m_fAngle = acosf(w / d);

	// acosf 함수가 구한 끼인각의 범위는 0 ~ 180도 이기에 다음과 같은 예외처리!
	if (m_tInfo.fY < m_pTarget->GetInfo().fY)
		m_fAngle *= -1.f;

	m_tInfo.fX -= cosf(m_fAngle) * m_fSpeed;
	m_tInfo.fY += sinf(m_fAngle) * m_fSpeed;
	explosionCount++;
	if (explosionCount == 150)
	{
		CGameObject::StopBGM(SOUNDMGR::MONSTER);
	}
	if (explosionCount > 300)
	{
		if (!explotion)
			headAnimationCount = 2;
		explotion = true;
		
	}
}
