#pragma once
#include "Scene.h"
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();
public:

	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;

private:
	float titleNameY;
};

