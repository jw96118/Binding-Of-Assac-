#pragma once
#include "GameObject.h"
class CShield :
	public CGameObject
{
public:
	CShield();
	virtual ~CShield();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;

private:
	virtual void Release() override;
public :
	void SetLayer(RENDER_LAYER _layer);

private:
	float			m_fDistance;
	CGameObject*	m_pTarget;
	unsigned int count;

	RENDER_LAYER layer;
};

