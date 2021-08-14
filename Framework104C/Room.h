#pragma once
class CFloor;
class CDoor;
class CRoom
{
public:
	CRoom();
	CRoom(wstring _floorTile, ROOM_TYPE _room_Type, POINT _pivot);
	~CRoom();
	void Initialize();

	int Update();

	void Render(HDC hDC);
private:
	void Release();

public:
	void SetPos(float _renderX, float _renderY,  POINT scr);
	void SetScroll(POINT scr);
	void SetPeripheryRoom(CRoom* cRoom, int i);
	void SetRoomType(ROOM_TYPE _room_Type);
	void SetRoomState(ROOM_STATE _room_State);
	void SetRenderMiniMpa(bool flag);
	void SetMiniMapPos(POINT pt);
	const FCol GetfloorInfo(int i) const;
	CRoom* GetPeripheryRoom(int i);
	bool GetRoomCheck(int i);
	POINT GetRenderP();
	const int& GetPeripheryRoomCount();
	ROOM_STATE GetRoom_State();
	const ROOM_TYPE& GetRoom_Type() const;
	const bool& GetRenderMiniMap() const;
	const POINT& GetMiniMapPos() const;
	RECT GetDoorCol(int i);
private:
	void RandomMake(bool room[]);
	void DeleteRoom(CRoom* room[]);
	void SetFloorPos();
	void MakeDoor();
	void DoorSetPos();
	void UpdateDoorCol();
private:
	wstring floorTile;
	ROOM_TYPE room_Type;
	ROOM_STATE room_State;
	CFloor* floorInfo[4];
	CDoor* doorInfo[4];
	RECT DoorCol[4];
	float renderX;
	float renderY;

	int tempCount = 0;
	CRoom* peripheryRoom[4];

	POINT pivot;
	int random;
	bool roomCheck[4];

	POINT testP;
	POINT scroll;
	POINT DoorColSize;
	bool makeDoorflag;
	int test = 0;
	Image* image;
	HDC hMemDC;

	bool renderMiniMap = false;

	POINT miniMapPos;
};

