#include "stdafx.h"
#include "MyBmp.h"


CMyBmp::CMyBmp()
{
}


CMyBmp::~CMyBmp()
{
	Release();
}

HDC CMyBmp::GetMemDC() const
{
	return m_hMemDC;
}

bool CMyBmp::LoadBmp(const wstring& wstrFilePath)
{
	// ��Ʈ�� �о����.
	m_hBitmap = (HBITMAP)LoadImage(nullptr, wstrFilePath.c_str(), IMAGE_BITMAP, 
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	NULL_CHECK_RETURN(m_hBitmap, false);

	HDC hDC = GetDC(g_hWnd);

	// CreateCompatibleDC: ���DC�� ȣȯ�� �Ǵ� �޸�DC�� �����ϴ� �Լ�.
	m_hMemDC = CreateCompatibleDC(hDC);	

	ReleaseDC(g_hWnd, hDC);
	NULL_CHECK_RETURN(m_hMemDC, false);

	// �޸�DC�� �ҷ��� ��Ʈ���� �̸� �׸���.
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	return true;
}

void CMyBmp::Release()
{
	// ���� DC�� ������ ����� �ٷ� ������ �� ���� �ع� �����־�� �Ѵ�.
	SelectObject(m_hMemDC, m_hOldBmp);

	// GDI ������Ʈ ����.
	DeleteObject(m_hBitmap);

	// �޸�DC ����.
	DeleteDC(m_hMemDC);
}
