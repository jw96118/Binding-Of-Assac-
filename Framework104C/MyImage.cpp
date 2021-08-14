#include "stdafx.h"
#include "MyImage.h"


CMyImage::CMyImage()
{
}


CMyImage::~CMyImage()
{
	Release();
}

HDC CMyImage::GetMemDC() const
{
	return m_hMemDC;
}

Image * CMyImage::GetImage()
{
	return image;
}

bool CMyImage::LoadMyImage(const WCHAR* wstrFilePath , int ImageEX, int ImageEY)
{
	image = Image::FromFile(wstrFilePath);
	NULL_CHECK_RETURN(image, false);
	HDC hDC = GetDC(g_hWnd);

	// CreateCompatibleDC: 출력DC와 호환이 되는 메모리DC를 생성하는 함수.
	m_hMemDC = CreateCompatibleDC(hDC);
	p = CreateCompatibleBitmap(hDC, WINCX, WINCY);
	op = (HBITMAP)SelectObject(m_hMemDC, p);
	ReleaseDC(g_hWnd, hDC);
	NULL_CHECK_RETURN(m_hMemDC, false);
	Gdiplus::Graphics g(m_hMemDC);
	g.DrawImage(image, Rect(0,0, ImageEX, ImageEY), 0, 0, ImageEX, ImageEY, UnitPixel);
	//TextOut(m_hMemDC, 100, 100, L"ASD", 3);
	return true;
}

bool CMyImage::LoadRoom()
{		
	image = Image::FromFile(L"Image/back/0d_barrenroom.png");
	NULL_CHECK_RETURN(image, false);
	HDC hDC = GetDC(g_hWnd);

	// CreateCompatibleDC: 출력DC와 호환이 되는 메모리DC를 생성하는 함수.
	m_hMemDC = CreateCompatibleDC(hDC);
	p = CreateCompatibleBitmap(hDC, WINCX, WINCY);
	op = (HBITMAP)SelectObject(m_hMemDC, p);
	ReleaseDC(g_hWnd, hDC);
	NULL_CHECK_RETURN(m_hMemDC, false);
	Gdiplus::Graphics g(m_hMemDC);
	g.DrawImage(image, Rect(0, 0, WINCX / 2, WINCY / 2), 0, 0, FLOORSIZEX, FLOORSIZEY, UnitPixel);
	g.DrawImage(image, Rect(WINCX , 0, -WINCX / 2, WINCY / 2), 0, 0, FLOORSIZEX, FLOORSIZEY, UnitPixel);
	g.DrawImage(image, Rect(0, WINCY , WINCX / 2, -WINCY / 2), 0, 0, FLOORSIZEX, FLOORSIZEY, UnitPixel);
	g.DrawImage(image, Rect(WINCX , WINCY , -WINCX / 2, -WINCY / 2), 0, 0, FLOORSIZEX, FLOORSIZEY, UnitPixel);
	image = Image::FromFile(L"Image/back/shading.png");
	NULL_CHECK_RETURN(image, false);
	g.DrawImage(image, Rect(0, 0, WINCX , WINCY ), 0, 0, 442, 286, UnitPixel);
	return true;
}



void CMyImage::Release()
{
	SelectObject(m_hMemDC, op);

	// GDI 오브젝트 해제.
	DeleteObject(p);


	DeleteDC(m_hMemDC);
}
