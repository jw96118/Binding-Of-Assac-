#include "stdafx.h"
#include "BmpMgr.h"
#include "MyBmp.h"

IMPLEMENT_SINGLETON(CBmpMgr)

CBmpMgr::CBmpMgr()
{
}


CBmpMgr::~CBmpMgr()
{
	Release();
}

HDC CBmpMgr::GetMemDC(const wstring& wstrImgKey) const
{
	auto iter_find = m_mapBmp.find(wstrImgKey);

	if (m_mapBmp.end() == iter_find)
		return nullptr;

	return iter_find->second->GetMemDC();
}

void CBmpMgr::LoadBmp(const wstring& wstrImgKey, const wstring& wstrFilePath)
{
	auto iter_find = m_mapBmp.find(wstrImgKey);

	if (m_mapBmp.end() != iter_find)
		return;

	CMyBmp* pBmp = new CMyBmp;

	if (!pBmp->LoadBmp(wstrFilePath))
	{
		SafeDelete(pBmp);
		MessageBox(nullptr, wstrFilePath.c_str(), L"Image Load Failed", MB_OK);
		return;
	}

	m_mapBmp.insert(make_pair(wstrImgKey, pBmp));
}

void CBmpMgr::Release()
{
	for (auto& MyPair : m_mapBmp)
		SafeDelete(MyPair.second);

	m_mapBmp.clear();
}
