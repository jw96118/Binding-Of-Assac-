#include "stdafx.h"
#include "GameSelect.h"


CGameSelect::CGameSelect()
{
}


CGameSelect::~CGameSelect()
{
}

void CGameSelect::Initialize()
{

	gameSelectState = NEW_RUN;
	CImageMgr::GetInstance()->AddImage(L"GAMESELECT", L"Image/ui/gamemenu.png",480,540);
}

int CGameSelect::Update()
{
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_SPACE) || CKeyMgr::GetInstance()->KeyPressing(KEY_RETURN))
	{

	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_UP))
	{
		dwCurTime = GetTickCount();
		if (dwCurTime - dwOldTime >= 250)
		{	
				gameSelectState = GAME_SELECT_STATE(gameSelectState - 1);
				if ((int)gameSelectState < 0)
				{
					gameSelectState = OPTIONS;
				}
			dwOldTime = dwCurTime;
		}
	}
	else if (CKeyMgr::GetInstance()->KeyPressing(KEY_DOWN))
	{
		dwCurTime = GetTickCount();
		if (dwCurTime - dwOldTime >= 250)
		{
			gameSelectState = GAME_SELECT_STATE(gameSelectState + 1);
			if ((int)gameSelectState == GAME_SELECT_STATE_END)
			{
				gameSelectState = NEW_RUN;
			}
			dwOldTime = dwCurTime;
		}
	}
	return 0;
}

void CGameSelect::Render(HDC hDC)
{
	

	hMemDC = CImageMgr::GetInstance()->GetDc(L"GAMESELECT");
	//image = CImageMgr::GetInstance()->GetImage(L"GAMESELECT");
	/*g.DrawImage(image, Rect((int)renderX, (int)renderY + WINCY * 2, WINCX, WINCY), 0, 0, 480, 270, UnitPixel);
	g.DrawImage(image, Rect((int)renderX + 450, (int)renderY + WINCY * 2 + 100, 440, 120), 35, 270, 125, 70, UnitPixel);
	g.DrawImage(image, Rect((int)renderX + 450 + (5 * 1),(int)renderY + WINCY * 2 + 200, 440, 140), 35, 270 + (70 * 1), 125, 50, UnitPixel);
	g.DrawImage(image, Rect((int)renderX + 450 + (5 * 2),(int)renderY + WINCY * 2 + 300, 440, 120), 35, 270 + 70 + (50 * 1), 125, 50, UnitPixel);
	g.DrawImage(image, Rect((int)renderX + 450 + (5 * 3),(int)renderY + WINCY * 2 + 400, 440, 120), 35, 270 + 65 + (50 * 2), 125, 50, UnitPixel);
	g.DrawImage(image, Rect((int)renderX + 450 + (5 * 5),(int)renderY + WINCY * 2 + 500, 440, 120), 35, 270 + 68 + (50 * 3), 125, 50, UnitPixel);
	g.DrawImage(image, Rect((int)renderX + 400 + (5 * (int)gameSelectState), (int)renderY + WINCY * 2 + 150 + (100 * (int)gameSelectState), 50, 70), 8, 310, 16, 20, UnitPixel);*/
	GdiTransparentBlt(hDC, (int)renderX, (int)renderY + WINCY , WINCX, WINCY, hMemDC, 0, 0, 480, 270, RGB(255, 0, 0));
	GdiTransparentBlt(hDC, (int)renderX + 450, (int)renderY + WINCY  + 100, 440, 120, hMemDC, 35, 270, 125, 70, RGB(0, 0, 0));
	GdiTransparentBlt(hDC, (int)renderX + 450 + (5 * 1), (int)renderY + WINCY  + 200, 440, 140, hMemDC, 35, 270 + (70 * 1), 125, 50, RGB(0, 0, 0));
	GdiTransparentBlt(hDC, (int)renderX + 450 + (5 * 2), (int)renderY + WINCY  + 300, 440, 120, hMemDC, 35, 270 + 70 + (50 * 1), 125, 50, RGB(0, 0, 0));
	GdiTransparentBlt(hDC, (int)renderX + 450 + (5 * 3), (int)renderY + WINCY + 400, 440, 120, hMemDC, 35, 270 + 65 + (50 * 2), 125, 50, RGB(0, 0, 0));
	GdiTransparentBlt(hDC, (int)renderX + 450 + (5 * 5), (int)renderY + WINCY  + 500, 440, 120, hMemDC, 35, 270 + 68 + (50 * 3), 125, 50, RGB(0, 0, 0));
	GdiTransparentBlt(hDC, (int)renderX + 400 + (5 * (int)gameSelectState), (int)renderY + WINCY  + 150 + (100 * (int)gameSelectState), 50, 70, hMemDC, 8, 310, 16, 20, RGB(0, 0, 0));

}

void CGameSelect::Release()
{
}

GAME_SELECT_STATE CGameSelect::GetSelectState() const
{
	return gameSelectState;
}
