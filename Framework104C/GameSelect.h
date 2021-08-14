#pragma once
#include "Scene.h"
class CGameSelect :
	public CScene
{
public:
	CGameSelect();
	~CGameSelect();

	// CScene��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;
public:
	GAME_SELECT_STATE GetSelectState() const;
private:
	GAME_SELECT_STATE gameSelectState;
	
};

