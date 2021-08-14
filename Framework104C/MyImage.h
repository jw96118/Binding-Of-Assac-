#pragma once
class CMyImage
{
public:
	CMyImage();
	~CMyImage();

public:
	HDC GetMemDC() const;
	Image* GetImage();
public:
	bool CMyImage::LoadMyImage(const WCHAR* wstrFilePath, int ImageEX, int ImageEY);
	bool CMyImage::LoadRoom();

private:
	void Release();

private:
	Image* image; 
	HBITMAP p, op;
	
	HDC		m_hMemDC; // �޸�DC: �ҷ��� ��Ʈ���� �̸� �׷����� DC
};



