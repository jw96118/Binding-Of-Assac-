#pragma once
#include "GameObject.h"
class CUI :
	public CGameObject
{
public:
	CUI();
	~CUI();

	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
private:
	CGameObject*	m_pTarget;
	bool debug = false;
};

