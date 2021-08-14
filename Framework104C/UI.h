#pragma once
#include "GameObject.h"
class CUI :
	public CGameObject
{
public:
	CUI();
	~CUI();

	// CGameObject��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
private:
	CGameObject*	m_pTarget;
	bool debug = false;
};

