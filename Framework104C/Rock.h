#pragma once
#include "GameObject.h"
class CRock : public CGameObject
{
public:
	CRock();
	~CRock();

	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;
public :
	void SetBroken(bool flag);
private:
	bool broken;
	bool brokenfly;
	int count;
	POINT broken1;
	POINT broken2;
	POINT moveBroken1;
	POINT moveBroken2;
};

