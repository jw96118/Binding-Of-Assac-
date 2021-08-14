#pragma once
#include "GameObject.h"
class CDoor :
	public CGameObject
{
public:
	CDoor();
	~CDoor();

	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;

public:
	void SetDoor(const wstring& type);
	void SetScroll(POINT pt);
	void SetAngle(float ang);
	void SetState(ROOM_STATE state);
private:
	wstring DoorType;
	POINT scroll;
	float angle;
	ROOM_STATE room_State;
	ROOM_STATE preroom_State;
};

