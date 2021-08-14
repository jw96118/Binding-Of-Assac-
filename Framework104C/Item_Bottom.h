#pragma once
#include "GameObject.h"
class CItem_Bottom :
	public CGameObject
{
public:
	CItem_Bottom();
	~CItem_Bottom();

	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;
private:
	int renderX;
	int renderY;
};

