#include "stdafx.h"
#include "Bomb.h"


CBomb::CBomb()
{
}


CBomb::~CBomb()
{
}

void CBomb::Initialize()
{

	CImageMgr::GetInstance()->AddImage(L"BOMB", L"Image/item/pickup_016_bomb.png", 192, 192);

	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;

	deadX = 0;
	deadY = 0;
	deadCount = 0;

	aniWidth = 0;
	aniHeight = 0;
	explosionCount = 0;

	type = BOMB;
	updownFlag = false;
	count = 0;
	explosion = false;
	imageSize.x = 80;
	imageSize.y = 80;
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;
}

int CBomb::Update()
{
	if (deadCount > 12)
	{
		return DEAD_OBJ;
	}
	if (explosionflagCount > 30)
	{
		explosion = true;
		type = EXPLOSION;
	}
	if (explosion)
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
		
		explosionCount++;
		imageSize.x = 96 * 2;
		imageSize.y = 96 * 2;
	}
	CObjectMgr::GetInstance()->AddLayer(LAYER_OBJECT, this);
	
	return NO_EVENT;
}

void CBomb::Render(HDC hDC)
{
	CGameObject::UpdateRect();
	int renderX = (int)m_tInfo.fX - imageSize.x / 2 + (int)g_fScrollX;
	int renderY = (int)m_tInfo.fY - imageSize.y / 2 + (int)g_fScrollY;
	if (explosion)
	{
		CGameObject::PlayerSound(L"boss explosions 0.wav", SOUNDMGR::EFFECT);
		hMemDC = CImageMgr::GetInstance()->GetDc(L"EXPLOSION");

		CGameObject::UpdateRect();
		GdiTransparentBlt(hDC, renderX, renderY - 70, imageSize.x, imageSize.y, hMemDC, EXPLOSIONIMAGEX*deadX, EXPLOSIONIMAGEY*deadY, EXPLOSIONIMAGEX, EXPLOSIONIMAGEY, RGB(0, 0, 0));
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
		hMemDC = CImageMgr::GetInstance()->GetDc(L"BOMB");
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 40)
		{
			if (!updownFlag)
			{
				imageSize.x += 1.f;
			}
			else
			{
				imageSize.x -= 1.f;
			}
			count++;
			if (count % 10 == 0 && count != 0)
			{
				updownFlag = !updownFlag;
			}
			explosionflagCount++;
			dwOldTime_Render = dwCurTime_Render;
		}
		GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, 0, 0, 32, 32, RGB(0, 0, 0));
	}
	//Rectangle(hDC, m_tRect.left + g_fScrollX, m_tRect.top + g_fScrollY, m_tRect.right + g_fScrollX, m_tRect.bottom + g_fScrollY);
}

void CBomb::Release()
{
}
