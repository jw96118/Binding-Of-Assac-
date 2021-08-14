#include "stdafx.h"
#include "Line.h"


CLine::CLine()
{
	ZeroMemory(&m_tInfo, sizeof(LINE_INFO));
}

CLine::CLine(const LINE_POS& rStart, const LINE_POS& rEnd)
{
	m_tInfo.tStart = rStart;
	m_tInfo.tEnd = rEnd;
}


CLine::~CLine()
{
}

const LINE_INFO& CLine::GetInfo() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_tInfo;
}

void CLine::Render(HDC hDC)
{
	// 화면에 그릴 때 스크롤 값을 적용해서 화면 좌표 안에 가둔다.
	MoveToEx(hDC, 
		(int)m_tInfo.tStart.fX - g_fScrollX, 
		(int)m_tInfo.tStart.fY, nullptr);
	LineTo(hDC, 
		(int)m_tInfo.tEnd.fX - g_fScrollX,
		(int)m_tInfo.tEnd.fY);
}
