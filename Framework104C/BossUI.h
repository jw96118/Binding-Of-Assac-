#pragma once
#include "GameObject.h"
class CBossUI :
	public CGameObject
{
public:
	CBossUI();
	~CBossUI();

	// CGameObject��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;
private:
	CGameObject*	m_pTarget;
	float boss_hp;
};

