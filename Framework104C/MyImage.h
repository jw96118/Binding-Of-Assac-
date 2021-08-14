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
	
	HDC		m_hMemDC; // 메모리DC: 불러온 비트맵을 미리 그려놓을 DC
};



