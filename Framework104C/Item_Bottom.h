#pragma once
#include "GameObject.h"
class CItem_Bottom :
	public CGameObject
{
public:
	CItem_Bottom();
	~CItem_Bottom();

	// CGameObject��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;
private:
	int renderX;
	int renderY;
};

