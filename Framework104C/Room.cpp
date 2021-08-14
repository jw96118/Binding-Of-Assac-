#include "stdafx.h"
#include "Room.h"
#include "Floor.h"
#include "Door.h"

CRoom::CRoom()
{
	for (int i = 0; i < 4; i++)
	{
		peripheryRoom[i] = nullptr;
		floorInfo[i] = nullptr;
		doorInfo[i] = nullptr;
		DoorCol[i] = { 0,0,0,0 };
	}
}

CRoom::CRoom(wstring _floorTile, ROOM_TYPE _room_Type, POINT _pivot)
{
	floorTile = _floorTile;
	room_Type = _room_Type;
	pivot = _pivot;
	for (int i = 0; i < 4; i++)
	{
		peripheryRoom[i] = nullptr;
		floorInfo[i] = nullptr;
		doorInfo[i] = nullptr;
		DoorCol[i] = { 0,0,0,0 };
	}

}

CRoom::~CRoom()
{
	Release();
}

void CRoom::Initialize()
{
	for (int i = 0; i < 4; i++)
	{
		floorInfo[i] = new CFloor();
		floorInfo[i]->Initialize();
		floorInfo[i]->SetTile(floorTile);
		roomCheck[i] = false;
	}
	RandomMake(roomCheck);
	testP.x = WINCX / 2;
	testP.y = WINCY / 2;
	renderX = 0;
	renderY = 0;
	scroll = { 0,0 };
	makeDoorflag = false;
	room_State = NO_BATTLE;

	CImageMgr::GetInstance()->AddImage();	
	miniMapPos = { 0,0 };
	srand((unsigned)time(NULL));
}

int CRoom::Update()
{

	if (!makeDoorflag)
	{
		MakeDoor();
		makeDoorflag = true;
	}
	SetFloorPos();
	for (int i = 0; i < 4; i++)
	{
		floorInfo[i]->Update();
		if (doorInfo[i] != nullptr)
		{
			doorInfo[i]->SetState(room_State);
			doorInfo[i]->Update();
		}
	}
	UpdateDoorCol();
	
	return 0;
}

void CRoom::Render(HDC hDC)
{
	Gdiplus::Graphics g(hDC);

	hMemDC = CImageMgr::GetInstance()->GetDc(L"ROOM_RENDER");
	GdiTransparentBlt(hDC, pivot.x + scroll.x, pivot.y + scroll.y, WINCX, WINCY,
		hMemDC, 0, 0, WINCX, WINCY, RGB(255, 255, 255));

	for (int i = 0; i < 4; i++)
	{
		if (doorInfo[i] != nullptr)
		{
			DoorSetPos();
			doorInfo[i]->Render(hDC);	
		}
	}
}

void CRoom::Release()
{
	for (int i = 0; i < 4; i++)
	{
		if (floorInfo[i] != nullptr)
		{
			delete floorInfo[i];
			floorInfo[i] = nullptr;
		}
		if (doorInfo[i] != nullptr)
		{
			delete doorInfo[i];
			doorInfo[i] = nullptr;
		}
	}
	//DeleteRoom(peripheryRoom);
}
void CRoom::SetPos(float _renderX, float _renderY, POINT scr)
{
	renderX = _renderX;
	renderY = _renderY;
	scroll = scr;
}

void CRoom::SetScroll(POINT scr)
{
	scroll = scr;
}

void CRoom::SetPeripheryRoom(CRoom * cRoom, int i)
{
	peripheryRoom[i] = cRoom;
}

void CRoom::SetRoomType(ROOM_TYPE _room_Type)
{
	room_Type = _room_Type;
}

void CRoom::SetRoomState(ROOM_STATE _room_State)
{
	room_State = _room_State;
}

void CRoom::SetRenderMiniMpa(bool flag)
{
	renderMiniMap = flag;
}

void CRoom::SetMiniMapPos(POINT pt)
{
	miniMapPos = pt;
}

const FCol CRoom::GetfloorInfo(int i) const
{
	return floorInfo[i]->GetCol();
}

CRoom * CRoom::GetPeripheryRoom(int i)
{
	return peripheryRoom[i];
}

bool CRoom::GetRoomCheck(int i)
{
	return roomCheck[i];
}

POINT CRoom::GetRenderP()
{
	return pivot;
}

const int& CRoom::GetPeripheryRoomCount()
{
	tempCount = 0;
	for (int i = 0; i < 4; i++)
	{
		if (peripheryRoom[i] != nullptr)
		{
			tempCount++;
		}
	}
	return tempCount;
}

ROOM_STATE CRoom::GetRoom_State()
{
	return room_State;
}

const ROOM_TYPE& CRoom::GetRoom_Type() const
{
	return room_Type;
}

const bool & CRoom::GetRenderMiniMap() const
{
	return renderMiniMap;
}

const POINT & CRoom::GetMiniMapPos() const
{
	return miniMapPos;
}

RECT CRoom::GetDoorCol(int i)
{
	return DoorCol[i];
}


void CRoom::RandomMake(bool room[])
{
	bool notMake = true;
	for (int i = 0; i < 4; i++)
	{
		random = rand() % 2;
		if (random == 0)
		{
			room[i] = true;
			notMake = false;
		}
	}
	while (notMake)
	{
		random = rand() % 4;
		if (peripheryRoom[random] == nullptr)
		{
			room[random] = true;
			break;
		}
		else
		{
			continue;
		}
	}

}

void CRoom::DeleteRoom(CRoom * room[])
{
	for (int i = 0; i < 4; i++)
	{
		if (room[i] != nullptr)
		{
			delete room[i];
			room[i] = nullptr;
		}
	}
}

void CRoom::SetFloorPos()
{
	int xSize = WALL_X_SIZE;
	int ySize = WALL_Y_SIZE;
	for (int i = 0; i < 4; i++)
	{
		POINT tempPivot = { 0,0 };
		POINT reverceImage = { 1,1 };
		RECT tempRect1 = { 0 };
		RECT tempRect2 = { 0 };
		int DoorX = 0;
		int DoorY = 0;
		switch (i)
		{
		case 0:
			if (peripheryRoom[LEFT] != nullptr)
			{
				DoorY = DOOR_SIZE;
			}
			if (peripheryRoom[TOP] != nullptr)
			{
				DoorX = DOOR_SIZE;
			}
			tempPivot.x = pivot.x;
			tempPivot.y = pivot.y;
			tempRect1 = RECT{ pivot.x,pivot.y, pivot.x + xSize , WINCY / 2 + pivot.y - DoorY };
			tempRect2 = RECT{ pivot.x,pivot.y, WINCX / 2 + pivot.x - DoorX, ySize + pivot.y };
			break;
		case 1:
			if (peripheryRoom[RIGHT] != nullptr)
			{
				DoorY = DOOR_SIZE;
			}
			if (peripheryRoom[TOP] != nullptr)
			{
				DoorX = DOOR_SIZE;
			}
			tempPivot.x = pivot.x + testP.x;
			tempPivot.y = pivot.y;
			reverceImage = { -1,1 };
			tempRect1 = RECT{ WINCX - xSize + pivot.x , pivot.y , WINCX + pivot.x, WINCY / 2 + pivot.y - DoorY };
			tempRect2 = RECT{ WINCX / 2 + pivot.x + DoorX, pivot.y, WINCX + pivot.x ,ySize + pivot.y };
			break;
		case 2:
			if (peripheryRoom[LEFT] != nullptr)
			{
				DoorY = DOOR_SIZE;
			}
			if (peripheryRoom[BOTTOM] != nullptr)
			{
				DoorX = DOOR_SIZE;
			}
			tempPivot.x = pivot.x;
			tempPivot.y = pivot.y + testP.y;
			reverceImage = { 1,-1 };
			tempRect1 = RECT{ pivot.x, WINCY / 2 + pivot.y + DoorY , xSize + pivot.x , WINCY + pivot.y };
			tempRect2 = RECT{ pivot.x, WINCY - ySize + pivot.y, WINCX / 2 + pivot.x - DoorX ,WINCY + pivot.y };
			break;
		case 3:
			if (peripheryRoom[RIGHT] != nullptr)
			{
				DoorY = DOOR_SIZE;
			}
			if (peripheryRoom[BOTTOM] != nullptr)
			{
				DoorX = DOOR_SIZE;
			}
			tempPivot.x = pivot.x + testP.x;
			tempPivot.y = pivot.y + testP.y;
			reverceImage = { -1,-1 };
			tempRect1 = RECT{ WINCX - xSize + pivot.x,WINCY / 2 + pivot.y + DoorY, WINCX + pivot.x, WINCY + pivot.y - DoorY };
			tempRect2 = RECT{ WINCX / 2 + pivot.x + DoorX ,WINCY - ySize + pivot.y , WINCX + pivot.x  ,WINCY + pivot.y };
			break;
		}
		floorInfo[i]->SetPos((float)tempPivot.x, (float)tempPivot.y, reverceImage.x, reverceImage.y, tempRect1, tempRect2, scroll);
	}
}

void CRoom::MakeDoor()
{
	for (int i = 0; i < 4; i++)
	{
		if (peripheryRoom[i] != nullptr)
		{
			doorInfo[i] = new CDoor();
			doorInfo[i]->Initialize();
			if (peripheryRoom[i]->GetRoom_Type() == BOSS_ROOM || room_Type == BOSS_ROOM)
			{
				doorInfo[i]->SetDoor(L"BOSS_DOOR");
			}
			else if (peripheryRoom[i]->GetRoom_Type() == GOLD_ROOM || room_Type == GOLD_ROOM)
			{
				doorInfo[i]->SetDoor(L"GOLD_DOOR");
			}
			else
			{
				doorInfo[i]->SetDoor(L"NOMAL_DOOR");
			}
			switch (i)
			{
			case LEFT:
				doorInfo[i]->SetPos(-(float)pivot.y - WINCY / 2 + DOOR_SIZE + 75, (float)pivot.x + 100);
				doorInfo[i]->SetAngle(-90.f);
				break;
			case RIGHT:
				doorInfo[i]->SetPos((float)pivot.y + WINCY / 2 - DOOR_SIZE - 105, -(float)pivot.x - WINCX + 225);
				doorInfo[i]->SetAngle(90.f);
				break;
			case TOP:
				doorInfo[i]->SetPos((float)pivot.x + WINCX / 2 - DOOR_SIZE - 50, (float)pivot.y + 25);
				doorInfo[i]->SetAngle(0.f);
				break;
			case BOTTOM:
				doorInfo[i]->SetPos(-(float)pivot.x - WINCX / 2 + DOOR_SIZE + 18, -(float)pivot.y - WINCY + 265);
				doorInfo[i]->SetAngle(180.f);
				break;
			}
		}
	}
}
void CRoom::DoorSetPos()
{
	for (int i = 0; i < 4; i++)
	{
		if (doorInfo[i] != nullptr)
		{
			switch (i)
			{
			case LEFT:
				doorInfo[i]->SetScroll(POINT{ -scroll.y, scroll.x });
				break;
			case RIGHT:
				doorInfo[i]->SetScroll(POINT{ scroll.y, -scroll.x });
				break;
			case TOP:
				doorInfo[i]->SetScroll(POINT{ scroll.x, scroll.y });
				break;
			case BOTTOM:
				doorInfo[i]->SetScroll(POINT{ -scroll.x, -scroll.y });
				break;
			}
		}
	}
}

void CRoom::UpdateDoorCol()
{

	for (int i = 0; i < 4; i++)
	{
		float tempX = 0;
		float tempY = 0;
		if (room_State == NOW_BATTLE)
		{
			DoorColSize.x = WALL_X_SIZE;
			DoorColSize.y = WALL_Y_SIZE;
		}
		else
		{
			if (room_State == NO_BATTLE)
			{
				DoorColSize.x = WALL_X_SIZE * 3 / 4;
				DoorColSize.y = WALL_Y_SIZE * 3 / 4;
			}
			else
			{
				DoorColSize.x = WALL_X_SIZE ;
				DoorColSize.y = WALL_Y_SIZE ;
			}
		}
		if (peripheryRoom[i] != nullptr)
		{
			switch (i)
			{
			case LEFT:
				tempX = (float)pivot.x + WINCX / 2 - WINCX / 2 ;
				tempY = (float)pivot.y + WINCY / 2 - DOOR_SIZE ;
				DoorCol[i] = { (int)tempX , (int)tempY,(int)tempX + DoorColSize.x, (int)tempY + DOOR_SIZE * 2 };
				break;
			case RIGHT:
				tempX = (float)pivot.x + WINCX / 2 + WINCX / 2 ;
				tempY = (float)pivot.y + WINCY / 2 - DOOR_SIZE ;
				DoorCol[i] = { (int)tempX - DoorColSize.x, (int)tempY,(int)tempX , (int)tempY + DOOR_SIZE * 2 };
				break;
			case TOP:
				tempX = (float)pivot.x + WINCX / 2 - DOOR_SIZE;
				tempY = (float)pivot.y + WINCY / 2 - WINCY / 2 ;
				DoorCol[i] = { (int)tempX , (int)tempY, (int)tempX + DOOR_SIZE * 2 , (int)tempY + DoorColSize.y };
				break;
			case BOTTOM:
				tempX = (float)pivot.x + WINCX / 2 - DOOR_SIZE ;
				tempY = (float)pivot.y + WINCY / 2 + WINCY / 2 ;
				DoorCol[i] = { (int)tempX , (int)tempY - DoorColSize.y,(int)tempX + DOOR_SIZE * 2 , (int)tempY };
				break;
			}

		}

	}

}
