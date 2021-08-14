#pragma once
#include "GameObject.h"
class CBomb :
	public CGameObject
{
public:
	CBomb();
	~CBomb();

	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
private:
	int deadX;
	int deadY;
	int deadCount;
	int explosionCount;
	int explosionflagCount;
	bool explosion;
	bool updownFlag;
	int count;
};

