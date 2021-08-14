#pragma once

class CLine;
class CGameObject;
class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();

public:
	void Initialize();
	void Render(HDC hDC);
	bool CollisionLine(CGameObject* pObject, float* pOutY);

private:
	void Release();

private:
	list<CLine*>	m_LineList;

public:
	static CLineMgr* GetInstance();
	void DestroyInstance();

private:
	static CLineMgr* m_pInstance;
};

