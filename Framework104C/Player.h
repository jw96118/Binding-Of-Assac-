#pragma once
#include "GameObject.h"

class CRoom;
class CP_Item;
class CPlayer :	public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	// CGameObject¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;

private:
	virtual void Release() override;

private:
	void KeyInput();
	void Animation(HDC hDC);
	void UpdateInvin();
	void UpdateStatus();
	//void Jump();
public:
	PLAYER_MOVE_STATE GetMoveState();
	CRoom* GetNowRoomInfo();
	PLAYER_INVIL_STATE GetInvilState();
	const bool& GetHaveItem() const;
	const bool& GetFate() const;
	const bool& GetItemUpdate() const;
	const float& GetMaxHp() const;
	const float& GetHp() const;
	const float& GetAtk() const;
	const int& GetBombSize() const;
	const STATUS& GetStatus() const;
	void SetMoveState(PLAYER_MOVE_STATE _moveState);
	void SetNowRoomInfo(CRoom* room);
	void SetScrollDir(ROOM_DIRECTION _scroll_Dir);
	void SetInvinState(PLAYER_INVIL_STATE _invinState);
	void SetHp(int _hp);
	void SetP_Item(CP_Item* _pItem);
	void SetHaveItem(bool flag);
	void SetBulletImage(wstring _image);
	void SetBulletImageSize(POINT _pt);
	void SetBulletColSize(float x,float y);
	void SetDoubleFlag(bool flag);
	void SetFateFlag(bool flag);
	void SetHeadFlag(bool flag);
	void SetAtk(float a);
	void SetShotSpeed(float a);
	void SetLayer(RENDER_LAYER _layer);
	void SetItemUpdate(bool flag);
	void SetPact(bool flag);
	
private:
	bool	m_bIsJump;
	float	m_fJumpForce;	// »˚
	float	m_fJumpAcc;		// ∞°º”µµ

	STATUS preStatus;
	STATUS nowStatus;
	unsigned int bulletCount;
	unsigned int bodyAnimationCount;
	unsigned int headAnimationCount;
	unsigned int attackCount;
	unsigned int scrollCount;
	unsigned int invinCount;
	PLAYER_STATE state;
	PLAYER_ATTACK_STATE atkState;
	PLAYER_MOVE_STATE moveState;
	PLAYER_INVIL_STATE invinState;
	int testcount=0;
	int renderX = 0;
	int renderY = 0; 
	int faterenderX = 0;
	int faterenderY = 0;
	CRoom* Now_Room_Info;
	ROOM_DIRECTION scroll_Dir;
	POINT moveTest;

	DWORD dwOldTime_invin = GetTickCount();
	DWORD dwCurTime_invin = 0;
	bool hurtAniflag;
	bool haveItem;
	vector<CP_Item*> p_ItemList;

	RENDER_LAYER layer;
	bool doubleflag;
	bool fateflag;
	bool headflag;
	bool pactflag;
	bool itemUpdate;
	//√—æÀ
	POINT bulletImageSize;
	float bulletColX;
	float bulletColY;
	wstring bulletImage;

	POINT fateImage;
	bool hurtSound =false;
	int boomSize;
	int shadowY;

	HDC HeadhMemDC;

	float testSound;
};

