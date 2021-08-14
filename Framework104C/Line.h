#pragma once
class CLine
{
public:
	CLine();
	CLine(const LINE_POS& rStart, const LINE_POS& rEnd);
	~CLine();

public:
	const LINE_INFO& GetInfo() const;

public:
	void Render(HDC hDC);

private:
	LINE_INFO	m_tInfo;
};

