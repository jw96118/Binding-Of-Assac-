#pragma once
#include "GameObject.h"
class CP_Item :
	public CGameObject
{
public:
	CP_Item();
	~CP_Item();

	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;

public:
	void SetColPlayer(bool flag);
	void SetItemType(P_ITEM_LIST _itemType);
	void PlayerUpdate();
private:
	void UpdateRect();
private:
	P_ITEM_LIST itemType = NON;
	CGameObject*	m_pTarget;
	wstring itemRender;
	unsigned int count;
	bool updownFlag;
	bool colPlayer;
	int renderX;
	int renderY;
};

