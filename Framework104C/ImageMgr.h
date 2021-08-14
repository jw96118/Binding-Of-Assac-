#pragma once

class CMyImage;
class CImageMgr
{
	DECLARE_SINGLETON(CImageMgr)
private:
	CImageMgr();
	~CImageMgr();


private:
	void Release();
public:
	void AddImage(const wstring& ImageKey, const WCHAR* wstrFilePath, int ImageEX, int ImageEY);
	void AddImage();
	void AddImage(const wstring& ImageKey, const WCHAR* wstrFilePath);
	HDC GetDc(const wstring& ImageKey) const;
	Image* GetImage(const wstring& ImageKey) const;
private:

	map<wstring, CMyImage*> imageList;
	map<wstring,Image*> imageListGdiP;
};

