#pragma once

class CMyBmp;
class CBmpMgr
{
	DECLARE_SINGLETON(CBmpMgr)

public:
	CBmpMgr();
	~CBmpMgr();

public:
	HDC GetMemDC(const wstring& wstrImgKey) const;

public:
	void LoadBmp(const wstring& wstrImgKey, const wstring& wstrFilePath);

private:
	void Release();

private:
	map<wstring, CMyBmp*>	m_mapBmp;
};

