#pragma once

class CMyBmp
{
public:
	CMyBmp();
	~CMyBmp();

public:
	HDC GetMemDC() const;

public:
	bool LoadBmp(const wstring& wstrFilePath);

private:
	void Release();

private:
	HBITMAP	m_hBitmap;
	HBITMAP	m_hOldBmp;
	HDC		m_hMemDC; // �޸�DC: �ҷ��� ��Ʈ���� �̸� �׷����� DC
};

