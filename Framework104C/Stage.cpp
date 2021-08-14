#include "stdafx.h"
#include "Stage.h"
#include "Room.h"
#include "Player.h"
#include "Boney.h"
#include "Charger.h"
#include "MaskAndHeart.h"
#include "Leech.h"
#include "Mulligan.h"
#include "Monstro.h"
#include "P_Item.h"
#include "Item_Bottom.h"
#include "MiniMap.h"
#include "Rock.h"
#include "BossUI.h"
#define ROOM_SIZE 8
CStage::CStage()
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	for (int i = 0; i < 4; i++)
		scrollCheck[i] = 0;
	CImageMgr::GetInstance()->AddImage(L"FLOOR_1", L"Image/back/0d_barrenroom.png", 520, 468);
	CImageMgr::GetInstance()->AddImage(L"CUT_BACK", L"Image/monstro/bgblack.png", 120, 68);
	CImageMgr::GetInstance()->AddImage(L"CUT_BNAME", L"Image/monstro/bossname_20.0_monstro.png", 192, 64);
	CImageMgr::GetInstance()->AddImage(L"CUT_PNAME", L"Image/monstro/playername_01_isaac.png", 192, 64);
	CImageMgr::GetInstance()->AddImage(L"CUT_BSPOT", L"Image/monstro/bossspot_02_cellar.png", 260, 70);
	CImageMgr::GetInstance()->AddImage(L"CUT_PLAYER", L"Image/monstro/playerportrait_01_isaac.png", 112, 78);
	CImageMgr::GetInstance()->AddImage(L"CUT_PSPOT", L"Image/monstro/playerspot_02_cellar.png", 172, 46);
	CImageMgr::GetInstance()->AddImage(L"CUT_MONSTRO", L"Image/monstro/portrait_20.0_monstro.png", 192, 192);
	CImageMgr::GetInstance()->AddImage(L"CUT_VS", L"Image/monstro/vs.png", 128, 64);
	while (MakeRandomRoom())
	{
		for_each(roomInfo.begin(), roomInfo.end(), SafeDelete<CRoom*>);
		roomInfo.clear();
	}
	CObjectMgr::GetInstance()->SetPlayerRoom(roomInfo[0]);

	scrollRoom = LEFT;

	SettingMonster();
	MakeItem();
	pCut.x = -500;
	pCut.y = 500;
	mCut.x = WINCX;
	mCut.y = 500;
	cutVs.x = WINCX / 2 - 128 * 1.5;
	cutVs.y = -200;
	startCut = false;
	cutCount = 0;
}

int CStage::Update()
{
	if (scrollFlag)
	{
		ScrollRoom();
	}
	for (unsigned int i = 0; i < roomInfo.size(); i++)
	{
		roomInfo[i]->SetScroll(POINT{ (int)g_fScrollX, (int)g_fScrollY });
		roomInfo[i]->Update();
	}
	playerInfo = CObjectMgr::GetInstance()->GetPlayer();

	CObjectMgr::GetInstance()->MonsterUpdate(dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo());
	if (dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->GetRoom_Type() != BOSS_ROOM)
	{
		CScene::PlayerBGM(L"the cellar alt.ogg");
	}
	return 0;
}

void CStage::Render(HDC hDC)
{
	if (startCut && !CObjectMgr::GetInstance()->BossCheck())
	{
		BossCutScene(hDC);
	}
	else
	{
		//MonsterCheck();
		if (dynamic_cast<CPlayer*>(playerInfo)->GetMoveState() == NO_MOVE)
		{
			dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->Render(hDC);
			//CObjectMgr::GetInstance()->MonsterRender(dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo(), hDC);

		}
		else if (dynamic_cast<CPlayer*>(playerInfo)->GetMoveState() == MOVE_ROOM)
		{

			dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->Render(hDC);
			dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->GetPeripheryRoom(scrollRoom)->Render(hDC);

			//CObjectMgr::GetInstance()->MonsterRender(dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo(), hDC);
			CObjectMgr::GetInstance()->MonsterRender(dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->GetPeripheryRoom(scrollRoom), hDC);

			if (dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->GetPeripheryRoom(scrollRoom) != nullptr)
			{
				if (dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->GetPeripheryRoom(scrollRoom)->GetRoom_Type() == BOSS_ROOM)
				{
					if (!CObjectMgr::GetInstance()->BossCheck())
						startCut = true;
				}
			}
		}
		for (unsigned int i = 2; i < roomInfo.size(); i++)
		{
			//roomInfo[i]->Render(hDC);	
			for (int j = 0; j < 4; j++)
			{
				Rectangle(hDC, roomInfo[i]->GetfloorInfo(j).COL_1.left, roomInfo[i]->GetfloorInfo(j).COL_1.top, roomInfo[i]->GetfloorInfo(j).COL_1.right, roomInfo[i]->GetfloorInfo(j).COL_1.bottom);
				Rectangle(hDC, roomInfo[i]->GetfloorInfo(j).COL_2.left, roomInfo[i]->GetfloorInfo(j).COL_2.top, roomInfo[i]->GetfloorInfo(j).COL_2.right, roomInfo[i]->GetfloorInfo(j).COL_2.bottom);
			}
		}
	}

}

void CStage::Release()
{
	for_each(roomInfo.begin(), roomInfo.end(), SafeDelete<CRoom*>);
	roomInfo.clear();
}

vector<CRoom*>& CStage::GetRoomInfo()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return roomInfo;
}

const bool & CStage::GetCutScene()
{
	return startCut;
}

void CStage::SetScroll(int i)
{
	scrollFlag = true;
	scrollRoom = (ROOM_DIRECTION)i;
	switch (i)
	{
	case LEFT:
		scrollCheck[LEFT] += 1;
		scrollCheck[RIGHT] -= 1;
		break;
	case RIGHT:
		scrollCheck[LEFT] -= 1;
		scrollCheck[RIGHT] += 1;
		break;
	case TOP:
		scrollCheck[TOP] += 1;
		scrollCheck[BOTTOM] -= 1;
		break;
	case BOTTOM:
		scrollCheck[TOP] -= 1;
		scrollCheck[BOTTOM] += 1;
		break;

	}
}

void CStage::SetCutScene(bool flag)
{
	startCut = flag;
}

bool CStage::MakeRandomRoom()
{
	int makeCount = 0;
	int InputRoom = 0;
	int i = 0;
	bool overlapflag = false;
	makeGoldRoomflag = false;
	makeBossRoomflag = false;
	while (true)
	{
		if (i == 0)
		{
			renderP = { 0,0 };
			roomT = START_ROOM;
			roomInfo.push_back(new CRoom(L"FLOOR_1", roomT, renderP));
			makeCount++;
			roomInfo[i]->Initialize();
		}
		else if (makeCount < ROOM_SIZE)
		{
			for (int j = 0; j < 4; j++)
			{
				overlapflag = false;

				if (roomInfo[i - 1]->GetRoomCheck(j) == true && roomInfo[i - 1]->GetPeripheryRoom(j) == nullptr && makeCount < ROOM_SIZE)
				{
					roomT = NOMAL_ROOM;
					switch (j)
					{
					case LEFT://left	
						renderP = { roomInfo[i - 1]->GetRenderP().x - WINCX,  roomInfo[i - 1]->GetRenderP().y };
						InputRoom = 1;
						break;
					case RIGHT://right
						renderP = { roomInfo[i - 1]->GetRenderP().x + WINCX,  roomInfo[i - 1]->GetRenderP().y };
						InputRoom = 0;
						break;
					case TOP://top
						renderP = { roomInfo[i - 1]->GetRenderP().x ,  roomInfo[i - 1]->GetRenderP().y - WINCY };
						InputRoom = 3;
						break;
					case BOTTOM://bottom
						renderP = { roomInfo[i - 1]->GetRenderP().x ,  roomInfo[i - 1]->GetRenderP().y + WINCY };
						InputRoom = 2;
						break;
					}
					for (int z = 0; z < makeCount; z++)
					{
						if (i - 1 != z)
						{
							if (renderP.x == roomInfo[z]->GetRenderP().x && renderP.y == roomInfo[z]->GetRenderP().y)
							{
								overlapflag = true;
								break;
							}
						}
					}
					if (overlapflag)
					{
						continue;
					}

					roomInfo.push_back(new CRoom(L"FLOOR_1", roomT, renderP));
					roomInfo[i - 1]->SetPeripheryRoom(roomInfo[makeCount], j);
					roomInfo[makeCount]->SetPeripheryRoom(roomInfo[i - 1], InputRoom);
					roomInfo[makeCount]->Initialize();
					makeCount++;
					// std::cout << i << " , " << roomInfo.size() << endl;
				}
			}
		}
		if (makeCount >= ROOM_SIZE)
		{
			break;
		}
		i++;
	}
	for (int j = 0; j < roomInfo.size(); j++)
	{
		for (int z = 0; z < roomInfo.size(); z++)
		{
			for (int w = 0; w < 4; w++)
			{
				if (roomInfo[j]->GetPeripheryRoom(w) == nullptr)
				{
					switch (w)
					{
					case LEFT:
						if (roomInfo[j]->GetRenderP().x - WINCX == roomInfo[z]->GetRenderP().x && roomInfo[j]->GetRenderP().y == roomInfo[z]->GetRenderP().y)
						{
							roomInfo[j]->SetPeripheryRoom(roomInfo[z], LEFT);
							if (roomInfo[z]->GetPeripheryRoom(RIGHT) == nullptr)
								roomInfo[z]->SetPeripheryRoom(roomInfo[j], RIGHT);
						}
						break;
					case RIGHT:
						if (roomInfo[j]->GetRenderP().x + WINCX == roomInfo[z]->GetRenderP().x && roomInfo[j]->GetRenderP().y == roomInfo[z]->GetRenderP().y)
						{
							roomInfo[j]->SetPeripheryRoom(roomInfo[z], RIGHT);
							if (roomInfo[z]->GetPeripheryRoom(LEFT) == nullptr)
								roomInfo[z]->SetPeripheryRoom(roomInfo[j], LEFT);
						}
						break;
					case TOP:
						if (roomInfo[j]->GetRenderP().x == roomInfo[z]->GetRenderP().x && roomInfo[j]->GetRenderP().y - WINCY == roomInfo[z]->GetRenderP().y)
						{
							roomInfo[j]->SetPeripheryRoom(roomInfo[z], TOP);
							if (roomInfo[z]->GetPeripheryRoom(BOTTOM) == nullptr)
								roomInfo[z]->SetPeripheryRoom(roomInfo[j], BOTTOM);
						}
						break;
					case BOTTOM:
						if (roomInfo[j]->GetRenderP().x == roomInfo[z]->GetRenderP().x && roomInfo[j]->GetRenderP().y + WINCY == roomInfo[z]->GetRenderP().y)
						{
							roomInfo[j]->SetPeripheryRoom(roomInfo[z], BOTTOM);
							if (roomInfo[z]->GetPeripheryRoom(TOP) == nullptr)
								roomInfo[z]->SetPeripheryRoom(roomInfo[j], TOP);
						}
						break;
					}
				}
			}
		}
	}

	int Pcount = 0;
	
	for (unsigned int j = 1; j < roomInfo.size(); j++)
	{
		if (roomInfo[j]->GetPeripheryRoomCount() > 1)
		{
			Pcount++;
		}
	}
	for (unsigned int j = 1; j < roomInfo.size(); j++)
	{
		if (roomInfo[j]->GetPeripheryRoomCount() <= 1)
		{
			if (!makeGoldRoomflag)
			{
				roomT = GOLD_ROOM;
				roomInfo[j]->SetRoomType(roomT);
				makeGoldRoomflag = true;
			}
			else if (!makeBossRoomflag)
			{
				roomT = BOSS_ROOM;
				roomInfo[j]->SetRoomType(roomT);
				makeBossRoomflag = true;
			}

		}
	}
	for (int j = 0; j < roomInfo.size(); j++)
	{
		if (j == 0)
		{
			roomInfo[j]->SetMiniMapPos(POINT{ 0,0 });		
		}
		for (int z = 0; z < 4; z++)
		{
			if (roomInfo[j]->GetPeripheryRoom(z) != nullptr)
			{
				switch (z)
				{
				case LEFT:
					roomInfo[j]->GetPeripheryRoom(z)->SetMiniMapPos(POINT{ roomInfo[j]->GetMiniMapPos().x - 18,roomInfo[j]->GetMiniMapPos().y });
					break;
				case RIGHT:
					roomInfo[j]->GetPeripheryRoom(z)->SetMiniMapPos(POINT{ roomInfo[j]->GetMiniMapPos().x + 18,roomInfo[j]->GetMiniMapPos().y });
					break;
				case TOP:
					roomInfo[j]->GetPeripheryRoom(z)->SetMiniMapPos(POINT{ roomInfo[j]->GetMiniMapPos().x,roomInfo[j]->GetMiniMapPos().y - 16 });
					break;
				case BOTTOM:
					roomInfo[j]->GetPeripheryRoom(z)->SetMiniMapPos(POINT{ roomInfo[j]->GetMiniMapPos().x,roomInfo[j]->GetMiniMapPos().y + 16 });
					break;
				}
			}
		}
	}

	//체크
	int testcountX = 0;
	int testcountY = 0;
	for (unsigned int j = 1; j < roomInfo.size(); j++)
	{
		if (roomInfo[0]->GetRenderP().x == roomInfo[j]->GetRenderP().x)
		{
			testcountX++;
		}
		else if (roomInfo[0]->GetRenderP().y == roomInfo[j]->GetRenderP().y)
		{
			testcountY++;
		}

	}
	if (testcountX >= ROOM_SIZE - 1)
	{
		return true;
	}
	else if (testcountY >= ROOM_SIZE - 1)
	{
		return true;
	}
	else if (Pcount > ROOM_SIZE - 2)
	{
		return true;
	}
	else if (roomInfo.size() != ROOM_SIZE)
	{
		return true;
	}
	else if (makeGoldRoomflag == false ||
		makeBossRoomflag == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CStage::ScrollRoom()
{
	CObjectMgr::GetInstance()->SetPlayerMoveDirection(scrollRoom);
	if (dynamic_cast<CPlayer*>(playerInfo)->GetHaveItem())
	{
		dynamic_cast<CPlayer*>(playerInfo)->SetItemUpdate(true);
	}
	dynamic_cast<CPlayer*>(playerInfo)->SetHaveItem(false);
	switch (scrollRoom)
	{
	case LEFT:
		g_fScrollX += SCROLL_SPEED_X;
		CObjectMgr::GetInstance()->SetScroll(true);
		if (g_fScrollX >= WINCX * (float)scrollCheck[scrollRoom])
		{
			g_fScrollX = WINCX * (float)scrollCheck[scrollRoom];
			dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->SetMoveState(NO_MOVE);
			dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->SetNowRoomInfo(dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->GetNowRoomInfo()->GetPeripheryRoom(scrollRoom));
			dynamic_cast<CMiniMap*>(CObjectMgr::GetInstance()->GetMiniMap())->SetMiniMapScroll(POINT{ 18,0 });
			//CObjectMgr::GetInstance()->SetPlayerMoveState(NO_MOVE);
			//CObjectMgr::GetInstance()->SetPlayerRoom(dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->GetPeripheryRoom(scrollRoom));
			CObjectMgr::GetInstance()->SetScroll(false);
			scrollFlag = false;
		}
		break;
	case RIGHT:
		g_fScrollX -= SCROLL_SPEED_X;
		CObjectMgr::GetInstance()->SetScroll(true);
		if (g_fScrollX <= -WINCX * (float)scrollCheck[scrollRoom])
		{
			g_fScrollX = -WINCX * (float)scrollCheck[scrollRoom];
			dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->SetMoveState(NO_MOVE);
			dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->SetNowRoomInfo(dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->GetNowRoomInfo()->GetPeripheryRoom(scrollRoom));
			dynamic_cast<CMiniMap*>(CObjectMgr::GetInstance()->GetMiniMap())->SetMiniMapScroll(POINT{ -18,0 });
			CObjectMgr::GetInstance()->SetScroll(false);
			scrollFlag = false;
		}
		break;
	case TOP:
		g_fScrollY += SCROLL_SPEED_Y;
		CObjectMgr::GetInstance()->SetScroll(true);
		if (g_fScrollY >= WINCY * (float)scrollCheck[scrollRoom])
		{
			g_fScrollY = WINCY * (float)scrollCheck[scrollRoom];
			dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->SetMoveState(NO_MOVE);
			dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->SetNowRoomInfo(dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->GetNowRoomInfo()->GetPeripheryRoom(scrollRoom));
			dynamic_cast<CMiniMap*>(CObjectMgr::GetInstance()->GetMiniMap())->SetMiniMapScroll(POINT{ 0,16 });
			//CObjectMgr::GetInstance()->SetPlayerMoveState(NO_MOVE);
			//CObjectMgr::GetInstance()->SetPlayerRoom(dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->GetPeripheryRoom(scrollRoom));
			CObjectMgr::GetInstance()->SetScroll(false);
			scrollFlag = false;
		}
		break;
	case BOTTOM:
		g_fScrollY -= SCROLL_SPEED_Y;
		CObjectMgr::GetInstance()->SetScroll(true);
		if (g_fScrollY <= -WINCY * (float)scrollCheck[scrollRoom])
		{
			g_fScrollY = -WINCY * (float)scrollCheck[scrollRoom];
			dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->SetMoveState(NO_MOVE);
			dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->SetNowRoomInfo(dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->GetNowRoomInfo()->GetPeripheryRoom(scrollRoom));
			dynamic_cast<CMiniMap*>(CObjectMgr::GetInstance()->GetMiniMap())->SetMiniMapScroll(POINT{ 0,-16 });
			CObjectMgr::GetInstance()->SetScroll(false);
			scrollFlag = false;
		}
		break;
	}



}


void CStage::MonsterCheck()
{
	auto iter_begin = CObjectMgr::GetInstance()->GetMonster().begin();
	auto iter_end = CObjectMgr::GetInstance()->GetMonster().end();
	for (; iter_begin != iter_end; ++iter_begin)
	{
		if ((*iter_begin)->GetInfo().fX > dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->GetRenderP().x && (*iter_begin)->GetInfo().fX < dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->GetRenderP().x + WINCX
			&& (*iter_begin)->GetInfo().fY > dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->GetRenderP().y && dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->GetRenderP().y + WINCY)
		{
			if (dynamic_cast<CMonster*>(*iter_begin)->GetState() != MONSTER_DEAD)
			{
				dynamic_cast<CPlayer*>(playerInfo)->GetNowRoomInfo()->SetRoomState(NOW_BATTLE);
			}
			else
			{
			}
		}
	}
}

void CStage::SettingMonster()
{
	int count = 0;
	for (int i = 0; i < 5; i++)
	{
		while (true)
		{
			count++;
			if (roomInfo[count]->GetRoom_Type() != GOLD_ROOM && roomInfo[count]->GetRoom_Type() != BOSS_ROOM)
			{
				switch (i)
				{
				case 0:
					CObjectMgr::GetInstance()->AddObject(MONSTER, CObjectFactory<CBoney>::CreateObject((float)roomInfo[count]->GetRenderP().x + 500, (float)roomInfo[count]->GetRenderP().y + 500, BONEY));
					CObjectMgr::GetInstance()->AddObject(MONSTER, CObjectFactory<CBoney>::CreateObject((float)roomInfo[count]->GetRenderP().x + 700, (float)roomInfo[count]->GetRenderP().y + 200, BONEY));
					break;
				case 1:
					CObjectMgr::GetInstance()->AddObject(MONSTER, CObjectFactory<CCharger>::CreateObject((float)roomInfo[count]->GetRenderP().x + 500, (float)roomInfo[count]->GetRenderP().y + 500, CHARGER));
					CObjectMgr::GetInstance()->AddObject(MONSTER, CObjectFactory<CCharger>::CreateObject((float)roomInfo[count]->GetRenderP().x + 700, (float)roomInfo[count]->GetRenderP().y + 200, CHARGER));
					break;
				case 2:
					CObjectMgr::GetInstance()->AddObject(MONSTER, CObjectFactory<CMaskAndHeart>::CreateObject((float)roomInfo[count]->GetRenderP().x + 900, (float)roomInfo[count]->GetRenderP().y + 200, MASKANDHEART));
					CObjectMgr::GetInstance()->AddObject(MONSTER, CObjectFactory<CMaskAndHeart>::CreateObject((float)roomInfo[count]->GetRenderP().x + 200, (float)roomInfo[count]->GetRenderP().y + 600, MASKANDHEART));
					break;
				case 3:
					CObjectMgr::GetInstance()->AddObject(MONSTER, CObjectFactory<CLeech>::CreateObject((float)roomInfo[count]->GetRenderP().x + 900, (float)roomInfo[count]->GetRenderP().y + 200, LEECH));
					CObjectMgr::GetInstance()->AddObject(MONSTER, CObjectFactory<CLeech>::CreateObject((float)roomInfo[count]->GetRenderP().x + 200, (float)roomInfo[count]->GetRenderP().y + 600, LEECH));
					break;
				case 4:
					CObjectMgr::GetInstance()->AddObject(MONSTER, CObjectFactory<CMulligan>::CreateObject((float)roomInfo[count]->GetRenderP().x + 900, (float)roomInfo[count]->GetRenderP().y + 200, MULLIGAN));
					CObjectMgr::GetInstance()->AddObject(MONSTER, CObjectFactory<CMulligan>::CreateObject((float)roomInfo[count]->GetRenderP().x + 200, (float)roomInfo[count]->GetRenderP().y + 600, MULLIGAN));

				default:
					break;
				}
				break;
			}

		}

	}
	for (int i = 0; i < roomInfo.size(); i++)
	{
		if (roomInfo[i]->GetRoom_Type() == BOSS_ROOM)
		{
			CObjectMgr::GetInstance()->AddObject(MONSTER, CObjectFactory<CMonstro>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2, (float)roomInfo[i]->GetRenderP().y + WINCY / 2, MONSTRO));

		}
	}
}

void CStage::BossCutScene(HDC hDC)
{
	hMemDC = CImageMgr::GetInstance()->GetDc(L"CUT_BACK");
	CScene::StopBGM(SOUNDMGR::BGM);
	GdiTransparentBlt(hDC, 0, 0, WINCX, WINCY,
		hMemDC, 0, 0, 120, 68, RGB(255, 255, 255));
	if (cutCount ==0)
	{
		CSoundMgr::GetInstance()->PlaySound(L"castleportcullis.wav" , SOUNDMGR::SCENE);
		CSoundMgr::GetInstance()->PlaySound(L"boss fight intro jingle_01.ogg", SOUNDMGR::SCENE);
	}
	if (cutCount <= 30)
	{
		pCut.x += 20;

		mCut.x -= 20;
		pCutName = pCut;
		mCutName = mCut;
	}
	else if (cutCount <= 100)
	{
		pCut.x += 1;

		mCut.x -= 1;
		if (cutVs.y < 100)
		{
			cutVs.y += 20;
		}
		else
		{
			cutVs.y = 100;
		}
	}
	else if (cutCount > 100)
	{
		pCut.x -= 30;

		mCut.x += 30;
		pCutName = pCut;
		mCutName = mCut;
	}

	hMemDC = CImageMgr::GetInstance()->GetDc(L"CUT_PSPOT");
	GdiTransparentBlt(hDC, pCut.x, pCut.y, 172 * 2, 46 * 2,
		hMemDC, 0, 0, 172, 46, RGB(0, 0, 0));
	hMemDC = CImageMgr::GetInstance()->GetDc(L"CUT_PLAYER");
	GdiTransparentBlt(hDC, pCut.x + 60, pCut.y - 90, 112 * 2, 78 * 2,
		hMemDC, 0, 0, 112, 78, RGB(255, 0, 255));
	hMemDC = CImageMgr::GetInstance()->GetDc(L"CUT_PNAME");
	GdiTransparentBlt(hDC, pCutName.x, pCutName.y - 400, 192 * 2, 64 * 2,
		hMemDC, 0, 0, 192, 64, RGB(0, 0, 0));


	hMemDC = CImageMgr::GetInstance()->GetDc(L"CUT_BSPOT");
	GdiTransparentBlt(hDC, mCut.x, mCut.y, 260 * 2, 70 * 2,
		hMemDC, 0, 0, 260, 70, RGB(0, 0, 0));
	hMemDC = CImageMgr::GetInstance()->GetDc(L"CUT_MONSTRO");
	GdiTransparentBlt(hDC, mCut.x + 60, mCut.y - 210, 192 * 2, 192 * 2,
		hMemDC, 0, 0, 192, 192, RGB(255, 0, 255));
	hMemDC = CImageMgr::GetInstance()->GetDc(L"CUT_BNAME");
	GdiTransparentBlt(hDC, mCutName.x + 60, mCutName.y - 400, 192 * 2, 64 * 2,
		hMemDC, 0, 0, 192, 64, RGB(0, 0, 0));


	hMemDC = CImageMgr::GetInstance()->GetDc(L"CUT_VS");
	GdiTransparentBlt(hDC, cutVs.x, cutVs.y, 128 * 2, 64 * 2,
		hMemDC, 0, 0, 128, 64, RGB(0, 0, 0));
	cutCount++;
	if (cutCount > 125)
	{
		CObjectMgr::GetInstance()->SetScroll(false);
		startCut = false;
		CScene::PlayerBGM(L"basic boss fight.ogg");

		CObjectMgr::GetInstance()->AddObject(UI, CObjectFactory<CBossUI>::CreateObject());
	}
}

void CStage::MakeItem()
{
	CObjectMgr::GetInstance()->AddObject(ITEM_BOTTOM, CObjectFactory<CItem_Bottom>::CreateObject((float)roomInfo[0]->GetRenderP().x + WINCX / 2 + 200, (float)roomInfo[0]->GetRenderP().y + 500));
	CObjectMgr::GetInstance()->AddObject(P_ITEM, CObjectFactory<CP_Item>::CreateObject((float)roomInfo[0]->GetRenderP().x + WINCX/2 + 200, (float)roomInfo[0]->GetRenderP().y + 450, CRICKETSHEAD));

	CObjectMgr::GetInstance()->AddObject(ITEM_BOTTOM, CObjectFactory<CItem_Bottom>::CreateObject((float)roomInfo[0]->GetRenderP().x + WINCX / 2 - 200, (float)roomInfo[0]->GetRenderP().y + 500));
	CObjectMgr::GetInstance()->AddObject(P_ITEM, CObjectFactory<CP_Item>::CreateObject((float)roomInfo[0]->GetRenderP().x + WINCX / 2 - 200, (float)roomInfo[0]->GetRenderP().y + 450, TWOTWO));

	CObjectMgr::GetInstance()->AddObject(ITEM_BOTTOM, CObjectFactory<CItem_Bottom>::CreateObject((float)roomInfo[0]->GetRenderP().x + WINCX / 2, (float)roomInfo[0]->GetRenderP().y + 200));
	CObjectMgr::GetInstance()->AddObject(P_ITEM, CObjectFactory<CP_Item>::CreateObject((float)roomInfo[0]->GetRenderP().x + WINCX / 2, (float)roomInfo[0]->GetRenderP().y + 150, THEPACT));



	srand((unsigned)time(NULL));
	
	for (int i = 1; i < roomInfo.size() ; i++)
	{
		if (roomInfo[i]->GetRoom_Type() == GOLD_ROOM)
		{
			CObjectMgr::GetInstance()->AddObject(ITEM_BOTTOM, CObjectFactory<CItem_Bottom>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX/2 +200, (float)roomInfo[i]->GetRenderP().y + WINCY/2));
			CObjectMgr::GetInstance()->AddObject(P_ITEM, CObjectFactory<CP_Item>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX/2 + 200, (float)roomInfo[i]->GetRenderP().y + WINCY/2 - 50, FATE));
			CObjectMgr::GetInstance()->AddObject(ITEM_BOTTOM, CObjectFactory<CItem_Bottom>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 - 200, (float)roomInfo[i]->GetRenderP().y + WINCY / 2));
			CObjectMgr::GetInstance()->AddObject(P_ITEM, CObjectFactory<CP_Item>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 - 200, (float)roomInfo[i]->GetRenderP().y + WINCY / 2 -50, HALOOFFIES));

			CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WALL_X_SIZE + 35 , (float)roomInfo[i]->GetRenderP().y + WALL_Y_SIZE + 35));
			CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WALL_X_SIZE + 105, (float)roomInfo[i]->GetRenderP().y + WALL_Y_SIZE + 35));
			CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WALL_X_SIZE + 35, (float)roomInfo[i]->GetRenderP().y + WALL_Y_SIZE + 105));
			CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WALL_X_SIZE + 105, (float)roomInfo[i]->GetRenderP().y + WALL_Y_SIZE + 105));

		}
		else if (roomInfo[i]->GetRoom_Type() == NOMAL_ROOM)
		{		
			if (i < 4)
			{
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2, (float)roomInfo[i]->GetRenderP().y + WINCY / 2));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 + 70, (float)roomInfo[i]->GetRenderP().y + WINCY / 2));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 + 140, (float)roomInfo[i]->GetRenderP().y + WINCY / 2));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 - 70, (float)roomInfo[i]->GetRenderP().y + WINCY / 2));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 - 140, (float)roomInfo[i]->GetRenderP().y + WINCY / 2));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2, (float)roomInfo[i]->GetRenderP().y + WINCY / 2 + 70));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2, (float)roomInfo[i]->GetRenderP().y + WINCY / 2 + 140));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2, (float)roomInfo[i]->GetRenderP().y + WINCY / 2 - 140));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2, (float)roomInfo[i]->GetRenderP().y + WINCY / 2 - 70));
			}
			else
			{
			//	CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2, (float)roomInfo[i]->GetRenderP().y + WINCY / 2));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 + 70, (float)roomInfo[i]->GetRenderP().y + WINCY / 2));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 - 70, (float)roomInfo[i]->GetRenderP().y + WINCY / 2));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2, (float)roomInfo[i]->GetRenderP().y + WINCY / 2 + 70));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 + 70, (float)roomInfo[i]->GetRenderP().y + WINCY / 2 + 70));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 - 70, (float)roomInfo[i]->GetRenderP().y + WINCY / 2 + 70));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2, (float)roomInfo[i]->GetRenderP().y + WINCY / 2 - 70));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 + 70, (float)roomInfo[i]->GetRenderP().y + WINCY / 2 - 70));
				CObjectMgr::GetInstance()->AddObject(ROCK, CObjectFactory<CRock>::CreateObject((float)roomInfo[i]->GetRenderP().x + WINCX / 2 - 70, (float)roomInfo[i]->GetRenderP().y + WINCY / 2 - 70));

			}
		}
	}
}

