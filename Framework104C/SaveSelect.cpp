#include "stdafx.h"
#include "SaveSelect.h"


CSaveSelect::CSaveSelect()
{
}


CSaveSelect::~CSaveSelect()
{
}

void CSaveSelect::Initialize()
{
	CImageMgr::GetInstance()->AddImage(L"SAVESELECT", L"Image/ui/emptyscreen.png",480,270);
	CImageMgr::GetInstance()->AddImage(L"SAVESELECT_1", L"Image/ui/saveselectmenu.png", 480, 556);
}

int CSaveSelect::Update()
{
	
	return 0;
}

void CSaveSelect::Render(HDC hDC)
{
	hMemDC = CImageMgr::GetInstance()->GetDc(L"SAVESELECT");
	GdiTransparentBlt(hDC, (int)renderX, (int)renderY + WINCY, WINCX, WINCY, hMemDC, 0, 0, 480, 270, RGB(0, 0, 0));

	hMemDC = CImageMgr::GetInstance()->GetDc(L"SAVESELECT_1");
	GdiTransparentBlt(hDC, (int)renderX + 520, (int)renderY + WINCY, 260, 60, hMemDC, 20, 0, 150, 30, RGB(0, 0, 0));
	GdiTransparentBlt(hDC, (int)renderX, (int)renderY + WINCY + 100, WINCX, WINCY - 200, hMemDC, 0, 270, 480, 210, RGB(0, 0, 0));
	GdiTransparentBlt(hDC, (int)renderX - 30, (int)renderY + WINCY * 2 - 150, WINCX, 120, hMemDC, 0, 190, 480, 60, RGB(0, 0, 0));

	//image = CImageMgr::GetInstance()->GetImage(L"SAVESELECT");
	//g.DrawImage(image, Rect((int)renderX, (int)renderY + WINCY, WINCX, WINCY), 0, 0, 480, 270, UnitPixel);
	//image = CImageMgr::GetInstance()->GetImage(L"SAVESELECT_1");
	//g.DrawImage(image, Rect((int)renderX + 520, (int)renderY + WINCY, 260, 60), 20, 0, 150, 30, UnitPixel);
	//g.DrawImage(image, Rect((int)renderX, (int)renderY + WINCY + 100, WINCX, WINCY - 200), 0, 270, 480, 210, UnitPixel);
	//g.DrawImage(image, Rect((int)renderX - 30, (int)renderY + WINCY * 2 - 150, WINCX, 120), 0, 190, 480, 60, UnitPixel);
}							
void CSaveSelect::Release()
{
}
