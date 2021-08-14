#include "stdafx.h"
#include "MiniMap.h"
#include "Player.h"
#include "Room.h"
CMiniMap::CMiniMap()
{
}


CMiniMap::~CMiniMap()
{
}

void CMiniMap::Initialize()
{
	CImageMgr::GetInstance()->AddImage(L"MINIMAP_1", L"Image/ui/minimap1.png", 128, 256);
	CImageMgr::GetInstance()->AddImage(L"MINIMAP_1_1", L"Image/ui/minimap1.png", 128, 256);
	CImageMgr::GetInstance()->AddImage(L"MINIMAP_2", L"Image/ui/minimap2.png", 256, 256);
	imageSize.x = 53*4;
	imageSize.y = 47*4;
	m_tInfo.fX = 1200;
	m_tInfo.fY = 250;
	miniMapScroll = { 0,0 };
}

int CMiniMap::Update()
{

	m_pTarget = CObjectMgr::GetInstance()->GetPlayer();
	roomInfo = CSceneMgr::GetInstance()->GetRoomInfo();
	CObjectMgr::GetInstance()->AddLayer(LAYER_UI, this);

	return 0;
}

void CMiniMap::Render(HDC hDC)
{
	int renderX;
	int renderY;
	int x = 0;
	
	renderX = (int)m_tInfo.fX - (imageSize.x);
	renderY = (int)m_tInfo.fY - (imageSize.y);

	hMemDC = CImageMgr::GetInstance()->GetDc(L"MINIMAP_1_1");
	
	BLENDFUNCTION bf;
	bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
	bf.BlendFlags = 0; // 무조건 0이어야 한다
	bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미
	bf.SourceConstantAlpha = 127; // 투명도(투명 0 - 불투명 255)

	GdiAlphaBlend(hDC, renderX, renderY, imageSize.x, imageSize.y,
		hMemDC, 0, 0, 53, 47, bf);

	hMemDC = CImageMgr::GetInstance()->GetDc(L"MINIMAP_1");
	GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
		hMemDC, 0, 0, 53, 47, RGB(0, 0, 0));


	hMemDC = CImageMgr::GetInstance()->GetDc(L"MINIMAP_2");
	for (int i = 0; i < roomInfo.size(); i++)
	{
		miniMapPt = { (int)m_tInfo.fX - 110  + roomInfo[i]->GetMiniMapPos().x + miniMapScroll.x , (int)m_tInfo.fY - 110 + roomInfo[i]->GetMiniMapPos().y + miniMapScroll.y };

		if (roomInfo[i]->GetRenderMiniMap())
		{
			int y = 48;
			if (roomInfo[i] == dynamic_cast<CPlayer*>(m_pTarget)->GetNowRoomInfo())
			{
				y = 112;
			}
			GdiTransparentBlt(hDC, miniMapPt.x, miniMapPt.y, 18, 16,
				hMemDC, 108, y, 18, 16, RGB(1, 1, 1));		
			if (roomInfo[i]->GetRoom_Type() == GOLD_ROOM)
			{
				GdiTransparentBlt(hDC, miniMapPt.x -9, miniMapPt.y - 4, 35, 32,
					hMemDC, 61, 64, 18, 16, RGB(0, 0, 0));
			}
			else if (roomInfo[i]->GetRoom_Type() == BOSS_ROOM)
			{
				GdiTransparentBlt(hDC, miniMapPt.x - 9, miniMapPt.y - 8, 35, 32,
					hMemDC, 29, 78, 18, 16, RGB(0, 0, 0));
			}
		}
	}
}

void CMiniMap::Release()
{
}

void CMiniMap::SetMiniMapScroll(POINT _miniMapScroll)
{
	miniMapScroll.x += _miniMapScroll.x;
	miniMapScroll.y += _miniMapScroll.y;
}
