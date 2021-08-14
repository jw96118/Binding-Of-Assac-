#include "stdafx.h"
#include "Title.h"


CTitle::CTitle()
{
}


CTitle::~CTitle()
{
}

void CTitle::Initialize()
{
	dwOldTime = GetTickCount();
	dwCurTime = 0;
	titleNameY = 40;
	count = 0;
}

int CTitle::Update()
{
	dwCurTime = GetTickCount();
	if (dwCurTime - dwOldTime >= 50)
	{
		if (count < 10)
		{
			titleNameY += 1;
		}
		else if (count < 20)
		{
			titleNameY -= 1;
		}
		else if (count >= 20)
		{
			count = 0;
		}
		count++;
		dwOldTime = dwCurTime;
	}
	return 0;
}

void CTitle::Render(HDC hDC)
{
	//Graphics g(hDC);
	CImageMgr::GetInstance()->AddImage(L"TITLE", L"Image/ui/titlemenu.png",480,540);

	
	hMemDC = CImageMgr::GetInstance()->GetDc(L"TITLE");

	//image = CImageMgr::GetInstance()->GetImage(L"TITLE");

	//g.DrawImage(image, Rect((int)renderX + 370, (int)renderY + 250, 470, 470), 160 * (count / 2 % 2), 380, 160, 160, UnitPixel);
	//g.DrawImage(image, Rect((int)renderX, (int)renderY + (int)titleNameY, WINCX, WINCY / 3), 0, 280, 480, 100, UnitPixel);

	GdiTransparentBlt(hDC, (int)renderX, (int)renderY, WINCX, WINCY, hMemDC, 0, 0, 480, 270, RGB(0, 0, 0));
	GdiTransparentBlt(hDC, (int)renderX + 370, (int)renderY + 250, 470, 470, hMemDC, 160 * (count / 2 % 2), 380, 160, 160, RGB(0, 0, 0));
	GdiTransparentBlt(hDC, (int)renderX, (int)renderY + (int)titleNameY, WINCX, WINCY / 3, hMemDC, 0, 280, 480, 100, RGB(0, 0, 0));
}

void CTitle::Release()
{
}
