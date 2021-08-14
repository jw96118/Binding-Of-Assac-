#include "stdafx.h"
#include "LineMgr.h"
#include "Line.h"
#include "GameObject.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
{
}


CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{
	LINE_POS tLinePos[4] = 
	{
		{ 100.f, 500.f },
		{ 300.f, 500.f },
		{ 500.f, 300.f },
		{ 700.f, 300.f }
	};

	m_LineList.push_back(new CLine(tLinePos[0], tLinePos[1]));
	m_LineList.push_back(new CLine(tLinePos[1], tLinePos[2]));
	m_LineList.push_back(new CLine(tLinePos[2], tLinePos[3]));
}

void CLineMgr::Render(HDC hDC)
{
	for (auto pLine : m_LineList)
		pLine->Render(hDC);
}

bool CLineMgr::CollisionLine(CGameObject* pObject, float* pOutY)
{
	if (m_LineList.empty())
		return false;

	CLine* pTarget = nullptr;

	for (auto pLine : m_LineList)
	{
		if (pLine->GetInfo().tStart.fX <= pObject->GetInfo().fX &&
			pObject->GetInfo().fX <= pLine->GetInfo().tEnd.fX)
		{
			pTarget = pLine;
			break;
		}
	}

	NULL_CHECK_RETURN(pTarget, false);
	
	// 두 점을 지나는 직선의 방정식
	// y = (y2 - y1) / (x2 - x1) * (x - x1) + y1
	float x1 = pTarget->GetInfo().tStart.fX;
	float y1 = pTarget->GetInfo().tStart.fY;
	float x2 = pTarget->GetInfo().tEnd.fX;
	float y2 = pTarget->GetInfo().tEnd.fY;

	*pOutY = (y2 - y1) / (x2 - x1) * (pObject->GetInfo().fX - x1) + y1;

	return true;
}

void CLineMgr::Release()
{
	for_each(m_LineList.begin(), m_LineList.end(), SafeDelete<CLine*>);
	m_LineList.clear();
}

CLineMgr* CLineMgr::GetInstance()
{
	if (nullptr == m_pInstance)
		m_pInstance = new CLineMgr;

	return m_pInstance;
}

void CLineMgr::DestroyInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
