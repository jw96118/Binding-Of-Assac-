#include "stdafx.h"
#include "Rock.h"


CRock::CRock()
{
}


CRock::~CRock()
{
}

void CRock::Initialize()
{
	imageSize.x = 80;
	imageSize.y = 80;

	m_tInfo.fCX = 60.f;
	m_tInfo.fCY = 50.f;


	CImageMgr::GetInstance()->AddImage(L"ROCK", L"Image/grid/rocks_catacombs.png", 512, 512);


	dwOldTime_Render = GetTickCount();
	dwCurTime_Render = 0;

	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;


	type = ROCK;



	hMemDC = CImageMgr::GetInstance()->GetDc(L"ROCK");
	brokenfly = false;
	moveBroken1 = { 0,0 };
	moveBroken2 = { 0,0 };
}

int CRock::Update()
{
	CGameObject::UpdateRect();

	if (broken)
	{
		m_tInfo.fCX = 0.f;
		m_tInfo.fCY = 0.f;

		CObjectMgr::GetInstance()->AddLayer(LAYER_BACK, this);
	}
	else
	{
		CObjectMgr::GetInstance()->AddLayer(LAYER_OBJECT, this);
	}
	return NO_EVENT;
}

void CRock::Render(HDC hDC)
{
	int renderX;
	int renderY;

	renderX = (int)m_tInfo.fX - (imageSize.x) / 2 + g_fScrollX;
	renderY = (int)m_tInfo.fY - (imageSize.y) / 2 + g_fScrollY;

	broken1.x = (int)m_tInfo.fX - (imageSize.x) / 2 + g_fScrollX + moveBroken1.x;
	broken1.y = (int)m_tInfo.fY - (imageSize.y) / 2 + g_fScrollY + moveBroken1.y;
	broken2.x = (int)m_tInfo.fX - (imageSize.x) / 2 + g_fScrollX + moveBroken2.x;
	broken2.y = (int)m_tInfo.fY - (imageSize.y) / 2 + g_fScrollY + moveBroken2.y;
	if (broken)
	{
		CGameObject::PlayerSound(L"rock crumble 0.wav", SOUNDMGR::ROCK_BROKEN);
		GdiTransparentBlt(hDC, broken1.x, broken1.y, 40, 40,
			hMemDC, 32 * 3, 0, 16, 16, RGB(0, 0, 0));
		GdiTransparentBlt(hDC, broken2.x, broken2.y, 40, 40,
			hMemDC, 32 * 3, 0, 16, 16, RGB(0, 0, 0));
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 20)
		{
			count++;
			if (count < 9)
			{
				moveBroken1.x += 4;
				moveBroken1.y -= 4;
				moveBroken2.x -= 4;
				moveBroken2.y -= 4;

			}

			dwOldTime_Render = dwCurTime_Render;
		}
	}
	else
	{
		GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
			hMemDC, 32, 0, 32, 32, RGB(0, 0, 0));
	}
}

void CRock::Release()
{
}

void CRock::SetBroken(bool flag)
{
	broken = flag;
}
