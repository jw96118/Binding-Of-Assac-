#pragma once
#include "GameObject.h"
class CRoom;
class CMiniMap :
	public CGameObject
{
public:
	CMiniMap();
	~CMiniMap();

	// CGameObject��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;
public:
	void SetMiniMapScroll(POINT _miniMapScroll);
private:
	CGameObject*	m_pTarget;
	vector<CRoom*>  roomInfo;
	POINT miniMapPt;
	POINT miniMapScroll;
};

