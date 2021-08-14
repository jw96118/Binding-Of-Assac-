#include "stdafx.h"
#include "Floor.h"


CFloor::CFloor()
{
	Initialize();
}


CFloor::~CFloor()
{

}

void CFloor::Initialize()
{
	m_tInfo.fCX = WINCX/2.f;
	m_tInfo.fCY = WINCY / 2.f;
	m_tInfo.fX = WINCX / 2.f - m_tInfo.fCX/2.f;
	m_tInfo.fY = WINCY / 2.f - m_tInfo.fCY/2.f;
	reverceX = 0;
	reverceY = 0;
	renderX = 0;
	renderY = 0;
	scroll = { 0,0 };
	testp = { 0,0 };
}

int CFloor::Update()
{
	//image = CImageMgr::GetInstance()->GetImage(tile);
	hMemDC = CImageMgr::GetInstance()->GetDc(tile);

	UpdateCol();
	return 0;
}

void CFloor::Render(HDC hDC)
{
	HDC tempDc = CreateCompatibleDC(NULL);
	HBITMAP paper = CreateCompatibleBitmap(hDC, WINCX, WINCY);
	HBITMAP oldPaper = (HBITMAP)SelectObject(tempDc, paper);
	DeleteObject(paper);

	UpdateCol();
	
	if (reverceX == -1 && reverceY == 1)
	{
		testp.x = (int)m_tInfo.fCX;
		StretchBlt(tempDc, FLOORSIZEX, 0, -FLOORSIZEX, FLOORSIZEY, hMemDC, 0, 0, FLOORSIZEX, FLOORSIZEY, SRCCOPY);
		GdiTransparentBlt(hDC, (int)renderX + scroll.x  , (int)renderY + scroll.y , (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			tempDc, 1, 0, FLOORSIZEX, FLOORSIZEY, RGB(255, 0, 0));
	}
	if (reverceY == -1 && reverceX==1)
	{
		testp.y = (int)m_tInfo.fCY;
		StretchBlt(tempDc, 0, FLOORSIZEY, FLOORSIZEX, -FLOORSIZEY, hMemDC, 0, 0, FLOORSIZEX, FLOORSIZEY, SRCCOPY);
		GdiTransparentBlt(hDC, (int)renderX + scroll.x, (int)renderY + scroll.y, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			tempDc, 0, 1, FLOORSIZEX, FLOORSIZEY, RGB(255, 0, 0));
	}
	if (reverceY == -1 && reverceX == -1)
	{
		testp.y = (int)m_tInfo.fCY;
		StretchBlt(tempDc, FLOORSIZEX, FLOORSIZEY, -FLOORSIZEX, -FLOORSIZEY, hMemDC, 0, 0, FLOORSIZEX, FLOORSIZEY, SRCCOPY);
		GdiTransparentBlt(hDC, (int)renderX + scroll.x, (int)renderY + scroll.y, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			tempDc, 1, 1, FLOORSIZEX, FLOORSIZEY, RGB(255, 0, 0));
	}
	if (reverceX == 1 && reverceY == 1)
	{
		GdiTransparentBlt(hDC, (int)renderX + scroll.x , (int)renderY + scroll.y, ((int)m_tInfo.fCX), ((int)m_tInfo.fCY),
			hMemDC, 0, 0, FLOORSIZEX, FLOORSIZEY, RGB(255, 0, 0));
	}
	DeleteDC(tempDc);
	//g.DrawImage(image, Rect((int)renderX + scroll.x+ testp.x, (int)renderY + scroll.y + testp.y, ((int)m_tInfo.fCX * reverceX) , ((int)m_tInfo.fCY *reverceY) ), 0, 0, FLOORSIZEX, FLOORSIZEY, UnitPixel);
	
	/*GdiTransparentBlt(hDC, (int)renderX + scroll.x + testp.x, (int)renderY + scroll.y + testp.y, ((int)m_tInfo.fCX * reverceX), ((int)m_tInfo.fCY *reverceY),
		hMemDC, 0, 0, FLOORSIZEX, FLOORSIZEY, RGB(0, 0, 0));*/

	//Rectangle(hDC, renderX + scroll.x, renderY + scroll.y, (int)m_tInfo.fCX * reverceX + renderX + scroll.x, (int)m_tInfo.fCY *reverceY + renderY + scroll.y);

}

void CFloor::Release()
{
}

void CFloor::SetTile(const wstring & _tile)
{
	tile = _tile;
}

void CFloor::SetPos(float _renderX, float _renderY, int _reverceX, int _reverceY, RECT rect1, RECT rect2,POINT scr) 
{
	renderX = _renderX;
	renderY = _renderY;

	reverceX = _reverceX;
	reverceY = _reverceY;
	firstCol.COL_1 = rect1;
	firstCol.COL_2 = rect2;
	scroll = scr;
	//cout << "scroll : " << scroll.x << " , " << scroll.y << endl;
}

FCol CFloor::GetCol()
{
	return floorCol;
}


void CFloor::UpdateCol()
{
	/*floorCol.COL_1.left   = firstCol.COL_1.left + scroll.x;
	floorCol.COL_1.right  = firstCol.COL_1.right + scroll.x;
	floorCol.COL_1.top    = firstCol.COL_1.top + scroll.y;
	floorCol.COL_1.bottom = firstCol.COL_1.bottom + scroll.y;

	floorCol.COL_2.left   = firstCol.COL_2.left + scroll.x;
	floorCol.COL_2.right  = firstCol.COL_2.right + scroll.x;
	floorCol.COL_2.top    = firstCol.COL_2.top + scroll.y;
	floorCol.COL_2.bottom = firstCol.COL_2.bottom + scroll.y;*/

	floorCol.COL_1.left = firstCol.COL_1.left;
	floorCol.COL_1.right = firstCol.COL_1.right;
	floorCol.COL_1.top = firstCol.COL_1.top;
	floorCol.COL_1.bottom = firstCol.COL_1.bottom;

	floorCol.COL_2.left = firstCol.COL_2.left;
	floorCol.COL_2.right = firstCol.COL_2.right;
	floorCol.COL_2.top = firstCol.COL_2.top;
	floorCol.COL_2.bottom = firstCol.COL_2.bottom;
}

