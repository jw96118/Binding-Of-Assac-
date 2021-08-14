#include "stdafx.h"
#include "Door.h"

#define DOOR_IMAGE_SIZE_X 128
#define DOOR_IMAGE_SIZE_Y 240
CDoor::CDoor()
{
}


CDoor::~CDoor()
{
}

void CDoor::Initialize()
{
	CImageMgr::GetInstance()->AddImage(L"NOMAL_DOOR", L"Image/grid/door_01_normaldoor.png");
	CImageMgr::GetInstance()->AddImage(L"GOLD_DOOR", L"Image/grid/door_02_treasureroomdoor.png");
	CImageMgr::GetInstance()->AddImage(L"BOSS_DOOR", L"Image/grid/door_10_bossroomdoor.png");
	imageSize.x = 64;
	imageSize.y = 45;
	angle = 0.f;
	room_State = NO_BATTLE;
	preroom_State = room_State;
}

int CDoor::Update()
{
	image = CImageMgr::GetInstance()->GetImage(DoorType);
	return 0;
}

void CDoor::Render(HDC hDC)
{
	Gdiplus::Graphics g(hDC);
	Gdiplus::Matrix matrix;
	matrix.RotateAt(angle, Gdiplus::PointF(DOOR_IMAGE_SIZE_X / 2, DOOR_IMAGE_SIZE_Y / 2));
	g.SetTransform(&matrix);
	g.DrawImage(image, Gdiplus::Rect((int)m_tInfo.fX + scroll.x, (int)m_tInfo.fY + scroll.y, (int)(imageSize.x * 2.5), (int)(imageSize.y * 2.5)), imageSize.x, 0, imageSize.x, imageSize.y, UnitPixel);
	if (room_State == NOW_BATTLE)
	{
		CGameObject::PlayerSound(L"door heavy close.wav", SOUNDMGR::EFFECT);
		g.DrawImage(image, Gdiplus::Rect((int)m_tInfo.fX + scroll.x, (int)m_tInfo.fY + scroll.y, (int)(imageSize.x * 2.5), (int)(imageSize.y * 2.5)), 0, imageSize.y, imageSize.x, imageSize.y, UnitPixel);
		g.DrawImage(image, Gdiplus::Rect((int)m_tInfo.fX + scroll.x, (int)m_tInfo.fY + scroll.y, (int)(imageSize.x * 2.5), (int)(imageSize.y * 2.5)), imageSize.x, imageSize.y, imageSize.x, imageSize.y, UnitPixel);
		preroom_State = room_State;
	}
	if (preroom_State != room_State)
	{
		CGameObject::StopBGM(SOUNDMGR::EFFECT);
		CGameObject::PlayerSound(L"door heavy open.wav", SOUNDMGR::EFFECT);
		preroom_State = room_State;
	}
	g.DrawImage(image, Gdiplus::Rect((int)m_tInfo.fX + scroll.x, (int)m_tInfo.fY + scroll.y, (int)(imageSize.x * 2.5), (int)(imageSize.y * 2.5)), 0, 0, imageSize.x, imageSize.y, UnitPixel);

}

void CDoor::Release()
{
}

void CDoor::SetDoor(const wstring & type)
{
	DoorType = type;
}

void CDoor::SetScroll(POINT pt)
{
	scroll = pt;
}

void CDoor::SetAngle(float ang)
{
	angle = ang;
}

void CDoor::SetState(ROOM_STATE state)
{
	room_State = state;
}


