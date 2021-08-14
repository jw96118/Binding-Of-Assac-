#include "stdafx.h"
#include "Charger.h"


CCharger::CCharger()
{
}


CCharger::~CCharger()
{
}

void CCharger::Initialize()
{
	state = MONSTER_IDLE;
	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;
	
	CImageMgr::GetInstance()->AddImage(L"CHARGER", L"Image/monster/023.001_drownedcharger.png", 128, 128);
	CImageMgr::GetInstance()->AddImage(L"CHARGER_2", L"Image/monster/023.001_drownedcharger_2.png", 128, 128);
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
	aniCount = 0;
}

int CCharger::Update()
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

	return 0;
}

void CCharger::Render(HDC hDC)
{
	CGameObject::UpdateRect();

	renderX = (int)m_tInfo.fX - imageSize.x / 2 + (int)g_fScrollX;
	renderY = (int)m_tInfo.fY - imageSize.y / 2 + (int)g_fScrollY;
	Animation(hDC);
}


void CCharger::Release()
{
}

void CCharger::Animation(HDC hDC)
{
	HDC tempDc = CreateCompatibleDC(NULL);
	HBITMAP paper = CreateCompatibleBitmap(hDC, WINCX, WINCY);
	HBITMAP oldPaper = (HBITMAP)SelectObject(tempDc, paper);
	DeleteObject(paper);
	if (colBullet)
	{
		colBullet = false;
		hMemDC = CImageMgr::GetInstance()->GetDc(L"CHARGER_2");
	}
	else
		hMemDC = CImageMgr::GetInstance()->GetDc(L"CHARGER");
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
		case MONSTER_ATTACK_LEFT:
			break;
		case MONSTER_ATTACK_RIGHT:
			break;
		case MONSTER_ATTACK_TOP:
			break;
		case MONSTER_ATTACK_BOTTOM:
			break;
		case MONSTER_DEAD:
		
			break;
		default:
			break;
		}
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 100)
		{
			aniCount++;
			if (aniCount >= 4)
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
			StretchBlt(tempDc, SIZEX, 0, -SIZEX, SIZEY, hMemDC, SIZEX, SIZEY * 3, SIZEX, SIZEY, SRCCOPY);
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, tempDc, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_ATTACK_RIGHT:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, SIZEX , SIZEY * 3, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_ATTACK_TOP:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, SIZEX * 2, SIZEY * 3, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_ATTACK_BOTTOM:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, 0 , SIZEY * 3, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		}
	}
	else if (state == MONSTER_DEAD)
	{
		hMemDC = CImageMgr::GetInstance()->GetDc(L"MONSTER_DEAD");
		CGameObject::PlayerSound(L"meaty deaths 3.wav", SOUNDMGR::MONSTER_DEAD);

		m_tInfo.fCX = 0;
		m_tInfo.fCY = 0;
		CGameObject::UpdateRect();
		GdiTransparentBlt(hDC, renderX, renderY, 100, 100, hMemDC, DEADIMAGEX*deadX, DEADIMAGEY*deadY, DEADIMAGEX, DEADIMAGEY, RGB(0, 0, 0));
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

void CCharger::MonsterUpdate()
{

	if (state < MONSTER_ATTACK_LEFT)
	{
		m_fSpeed = 3.5f;
		if ((m_pTarget->GetInfo().fX <= m_tRect.right && m_pTarget->GetInfo().fX >= m_tRect.left)
			|| (m_pTarget->GetInfo().fY <= m_tRect.bottom && m_pTarget->GetInfo().fY >= m_tRect.top))
		{
			m_fSpeed = 6.f;
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
			CGameObject::PlayerSound(L"maggot1.wav", SOUNDMGR::MONSTER);
		}
		dwCurTime_Update = GetTickCount();
		if (dwCurTime_Update - dwOldTime_Update >= 250)
		{
			state = (MONSTER_STATE)(rand() % 4 + 1);
			dwOldTime_Update = dwCurTime_Update;
		}
		switch (state)
		{
		case MONSTER_IDLE:
			break;
		case MONSTER_MOVE_LEFT:	
			CGameObject::StopBGM(SOUNDMGR::MONSTER);
			m_tInfo.fX -= m_fSpeed;
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
