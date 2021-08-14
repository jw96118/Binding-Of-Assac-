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
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_tInfo;
}

void CLine::Render(HDC hDC)
{
	// ȭ�鿡 �׸� �� ��ũ�� ���� �����ؼ� ȭ�� ��ǥ �ȿ� ���д�.
	MoveToEx(hDC, 
		(int)m_tInfo.tStart.fX - g_fScrollX, 
		(int)m_tInfo.tStart.fY, nullptr);
	LineTo(hDC, 
		(int)m_tInfo.tEnd.fX - g_fScrollX,
		(int)m_tInfo.tEnd.fY);
}
