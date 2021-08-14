#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#define HEARTSIZEX 16
#define HEARTSIZEY 16
CUI::CUI()
{
}


CUI::~CUI()
{
}

void CUI::Initialize()
{
	CImageMgr::GetInstance()->AddImage(L"HEART_UI", L"Image/ui/ui_hearts.png", 112, 64);

	CImageMgr::GetInstance()->AddImage(L"BOMB", L"Image/item/pickup_016_bomb.png", 192, 192);
	imageSize.x = 32;
	imageSize.y = 32;
	m_tInfo.fX = 100;
	m_tInfo.fY = 50;
}

int CUI::Update()
{
	m_pTarget = CObjectMgr::GetInstance()->GetPlayer();
	CObjectMgr::GetInstance()->AddLayer(LAYER_UI, this);

	if (CKeyMgr::GetInstance()->KeyDown(KEY_Y))
	{
		debug = !debug;
	}
	return 0;
}

void CUI::Render(HDC hDC)
{
	int renderX;
	int renderY;
	int x = 0;
	hMemDC = CImageMgr::GetInstance()->GetDc(L"HEART_UI");
	for (int i = 0; i < dynamic_cast<CPlayer*>(m_pTarget)->GetMaxHp() / 2; i++)
	{
		renderX = (int)m_tInfo.fX - (imageSize.x) + 25*i;
		renderY = (int)m_tInfo.fY - (imageSize.y);

		if (dynamic_cast<CPlayer*>(m_pTarget)->GetHp() - 2 * i >= 2)
		{
			x = 0;
		}
		else if (dynamic_cast<CPlayer*>(m_pTarget)->GetHp() - 2 * i == 1)
		{
			x = 1;
		}
		else if (dynamic_cast<CPlayer*>(m_pTarget)->GetHp() - 2 * i <= 0)
		{
			x = 2;
		}
		GdiTransparentBlt(hDC,renderX , renderY , imageSize.x , imageSize.y,
			hMemDC, HEARTSIZEX * x, 0, HEARTSIZEX, HEARTSIZEY, RGB(0, 0, 0));
	}
	hMemDC = CImageMgr::GetInstance()->GetDc(L"BOMB");
	GdiTransparentBlt(hDC, (int)m_tInfo.fX - 50 , (int)m_tInfo.fY + 50, 40, 40, hMemDC, 0, 0, 32, 32, RGB(0, 0, 0));
	//SetBkColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC,TRANSPARENT);
	TCHAR sz[32] = L"";
	swprintf_s(sz, L"X %d", dynamic_cast<CPlayer*>(m_pTarget)->GetBombSize());
	TextOut(hDC, (int)m_tInfo.fX - 10, (int)m_tInfo.fY + 60, sz, lstrlen(sz));

	if (debug)
	{
	
		swprintf_s(sz, L"공격력 : %f", dynamic_cast<CPlayer*>(m_pTarget)->GetStatus().atk);
		TextOut(hDC, (int)m_tInfo.fX, (int)m_tInfo.fY + 80, sz, lstrlen(sz));

		swprintf_s(sz, L"연사   : %f", dynamic_cast<CPlayer*>(m_pTarget)->GetStatus().shotSpeed);
		TextOut(hDC, (int)m_tInfo.fX, (int)m_tInfo.fY + 100, sz, lstrlen(sz));

		swprintf_s(sz, L"이동속도 : %f", dynamic_cast<CPlayer*>(m_pTarget)->GetStatus().moveSpeed);
		TextOut(hDC, (int)m_tInfo.fX, (int)m_tInfo.fY + 120, sz, lstrlen(sz));
	}
}

void CUI::Release()
{
}
