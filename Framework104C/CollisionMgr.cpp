#include "stdafx.h"
#include "CollisionMgr.h"
#include "GameObject.h"
#include "Room.h"
#include "Bullet.h"
#include "Stage.h"
#include "Player.h"
#include "Monster.h"
#include "P_Item.h"
#include "Rock.h"
#include "Shield.h"
CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CollisionRect(OBJECT_LIST& dstList, OBJECT_LIST& srcList)
{
	for (auto pDest : dstList)
	{
		for (auto pSrc : srcList)
		{
			RECT rc = {};

			// 두 사각형이 교차되었는지 판별하는 함수. 교차됐으면 TRUE, 아니면 FALSE 반환.
			// 두 사각형이 교차되었을 때 겹친 영역에 또다른 사각형이 만들어진다. 
			// 이 겹친 사각형을 첫번째 인자로 반환한다.
			if (IntersectRect(&rc, &pDest->GetRect(), &pSrc->GetRect()))
			{
				if (pDest->GetObjType() == MONSTER_BULLET || pDest->GetObjType() == PLAYER_BULLET)
				{
					dynamic_cast<CBullet*>(pDest)->SetAniFlag(true);
				}
				else
				{
					pDest->IsDead();
					pSrc->IsDead();
				}
			}
		}
	}
}

void CCollisionMgr::CollisionRectEx(OBJECT_LIST& dstList, OBJECT_LIST& srcList)
{
	float fMoveX = 0.f, fMoveY = 0.f;

	for (auto pDest : dstList)
	{
		for (auto pSrc : srcList)
		{
			if (pDest->GetObjType() == MONSTER)
			{
				if (dynamic_cast<CMonster*>(pDest)->GetState() != MONSTER_DEAD)
				{
					if (IntersectRectEx(pDest, pSrc, &fMoveX, &fMoveY))
					{
						float fX = pSrc->GetInfo().fX;
						float fY = pSrc->GetInfo().fY;

						if (fMoveX > fMoveY) // Y축으로 밀어냄
						{
							if (pDest->GetInfo().fY < fY)
								pSrc->SetPos(fX, fY + 15);
							else
								pSrc->SetPos(fX, fY - 15);
						}
						else // X축으로 밀어냄
						{
							if (pDest->GetInfo().fX < fX)
								pSrc->SetPos(fX + 15, fY);
							else
								pSrc->SetPos(fX - 15, fY);
						}
					}
				}
			}
		}
	}
}

void CCollisionMgr::CollisionSphere(OBJECT_LIST& dstList, OBJECT_LIST& srcList)
{
	for (auto pDest : dstList)
	{
		for (auto pSrc : srcList)
		{
			if (IntersectSphere(pDest, pSrc))
			{
				pDest->IsDead();
				pSrc->IsDead();
			}
		}
	}
}

void CCollisionMgr::CollisionObject(OBJECT_LIST & dstList, OBJECT_LIST & srcList)
{
	POINT tempP = {};
	for (auto pDest : dstList)
	{
		for (auto pSrc : srcList)
		{
			RECT rc = {};

			if (IntersectRect(&rc, &pDest->GetRect(), &pSrc->GetRect()))
			{
				switch (pDest->GetObjType())
				{
				case PLAYER:
					if (pSrc->GetObjType() == P_ITEM)
					{
						if (!dynamic_cast<CPlayer*>(pDest)->GetHaveItem())
						{
							dynamic_cast<CPlayer*>(pDest)->SetHaveItem(true);
							dynamic_cast<CP_Item*>(pSrc)->SetColPlayer(true);
							dynamic_cast<CPlayer*>(pDest)->SetP_Item(dynamic_cast<CP_Item*>(pSrc));
						}
					}
					else if (pSrc->GetObjType() == ITEM_BOTTOM)
					{
						ColObject(pDest, pSrc->GetRect());
					}
					if (pSrc->GetObjType() == ROCK)
					{
						if (!dynamic_cast<CPlayer*>(pDest)->GetFate())
						{
							ColObject(pDest, pSrc->GetRect());
						}
						else
						{
							dynamic_cast<CPlayer*>(pDest)->SetLayer(LAYER_FLY_OBJECT);
							if (CObjectMgr::GetInstance()->GetShield1() != nullptr)
							{
								dynamic_cast<CShield*>(CObjectMgr::GetInstance()->GetShield1())->SetLayer(LAYER_FLY_OBJECT);
							}
							if (CObjectMgr::GetInstance()->GetShield2() != nullptr)
							{
								dynamic_cast<CShield*>(CObjectMgr::GetInstance()->GetShield2())->SetLayer(LAYER_FLY_OBJECT);
							}
						}
					}
					else
					{
						dynamic_cast<CPlayer*>(pDest)->SetLayer(LAYER_OBJECT);
						if (CObjectMgr::GetInstance()->GetShield1() != nullptr)
						{
							dynamic_cast<CShield*>(CObjectMgr::GetInstance()->GetShield1())->SetLayer(LAYER_OBJECT);
						}
						if (CObjectMgr::GetInstance()->GetShield2() != nullptr)
						{
							dynamic_cast<CShield*>(CObjectMgr::GetInstance()->GetShield2())->SetLayer(LAYER_OBJECT);
						}
					}
					break;
				case PLAYER_BULLET:
					if (pSrc->GetObjType() == ROCK)
					{
						dynamic_cast<CBullet*>(pDest)->SetAniFlag(true);
					}
					if (pSrc->GetObjType() == ITEM_BOTTOM)
					{
						dynamic_cast<CBullet*>(pDest)->SetAniFlag(true);
					}
					break;
				case SHIELD:
					if (pSrc->GetObjType() == MONSTER)
					{
						dynamic_cast<CMonster*>(pSrc)->SetHp(0.1f);
					}
					if (pSrc->GetObjType() == MONSTER_BULLET)
					{
						dynamic_cast<CBullet*>(pSrc)->SetAniFlag(true);
					}
					if (pSrc->GetObjType() == MONSTRO_BULLET)
					{
						dynamic_cast<CBullet*>(pSrc)->SetAniFlag(true);
					}if (pSrc->GetObjType() == BONY_BULLET)
					{
						dynamic_cast<CBullet*>(pSrc)->SetAniFlag(true);
					}
					break;
				case MONSTER:
					if (dynamic_cast<CMonster*>(pDest)->GetState() != MONSTER_DEAD)
					{
						if (pSrc->GetObjType() == PLAYER)
						{
							if (dynamic_cast<CPlayer*>(pSrc)->GetInvilState() == NOT_INVINCIBILITY)
							{
								dynamic_cast<CPlayer*>(pSrc)->SetHp(-1);
								dynamic_cast<CPlayer*>(pSrc)->SetInvinState(INVINCIBILITY);
							}
						}
						else if (pSrc->GetObjType() == PLAYER_BULLET)
						{
							dynamic_cast<CBullet*>(pSrc)->SetAniFlag(true);
							dynamic_cast<CMonster*>(pDest)->SetHp(dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->GetAtk());
							dynamic_cast<CMonster*>(pDest)->SetColBullet(true);
						}
						if (pSrc->GetObjType() == ROCK)
						{
							if (dynamic_cast<CMonster*>(pDest)->GetMonsterType() != LEECH)
							{
								ColObject(pDest, pSrc->GetRect());
								dynamic_cast<CMonster*>(pDest)->Refresh();
							}
						}
					}
					break;
				case MONSTER_BULLET:
					if (pSrc->GetObjType() == PLAYER)
					{
						if (dynamic_cast<CPlayer*>(pSrc)->GetInvilState() == NOT_INVINCIBILITY)
						{
							dynamic_cast<CPlayer*>(pSrc)->SetHp(-1);
							dynamic_cast<CPlayer*>(pSrc)->SetInvinState(INVINCIBILITY);
							dynamic_cast<CBullet*>(pDest)->SetAniFlag(true);
						}
					}
					if (pSrc->GetObjType() == ROCK)
					{
						dynamic_cast<CBullet*>(pDest)->SetAniFlag(true);
					}
					break;
				case MONSTRO_BULLET:
					if (pSrc->GetObjType() == PLAYER)
					{
						if (dynamic_cast<CPlayer*>(pSrc)->GetInvilState() == NOT_INVINCIBILITY)
						{
							dynamic_cast<CPlayer*>(pSrc)->SetHp(-1);
							dynamic_cast<CPlayer*>(pSrc)->SetInvinState(INVINCIBILITY);
							dynamic_cast<CBullet*>(pDest)->SetAniFlag(true);
						}
					}
					if (pSrc->GetObjType() == ROCK)
					{
						dynamic_cast<CBullet*>(pDest)->SetAniFlag(true);
					}
					break;
				case BONY_BULLET:
					if (pSrc->GetObjType() == PLAYER)
					{
						if (dynamic_cast<CPlayer*>(pSrc)->GetInvilState() == NOT_INVINCIBILITY)
						{
							dynamic_cast<CPlayer*>(pSrc)->SetHp(-1);
							dynamic_cast<CPlayer*>(pSrc)->SetInvinState(INVINCIBILITY);
							dynamic_cast<CBullet*>(pDest)->SetAniFlag(true);
						}
					}
					if (pSrc->GetObjType() == ROCK)
					{
						dynamic_cast<CBullet*>(pDest)->SetAniFlag(true);
					}
					break;
				case EXPLOSION:
					if (pSrc->GetObjType() == PLAYER)
					{
						if (dynamic_cast<CPlayer*>(pSrc)->GetInvilState() == NOT_INVINCIBILITY)
						{
							dynamic_cast<CPlayer*>(pSrc)->SetHp(-1);
							dynamic_cast<CPlayer*>(pSrc)->SetInvinState(INVINCIBILITY);
						}
					}
					if (pSrc->GetObjType() == MONSTER)
					{
						dynamic_cast<CMonster*>(pSrc)->SetHp(50);
						dynamic_cast<CMonster*>(pSrc)->SetColBullet(true);
					}
					if (pSrc->GetObjType() == ROCK)
					{
						dynamic_cast<CRock*>(pSrc)->SetBroken(true);
					}

					break;
				}
			}

			if (pDest->GetObjType() == MONSTER)
			{
				if (IntersectRect(&rc, &dynamic_cast<CMonster*>(pDest)->GetMonsterSRect(), &pSrc->GetRect()))
				{
					if (pSrc->GetObjType() == PLAYER)
					{
						if (dynamic_cast<CPlayer*>(pSrc)->GetInvilState() == NOT_INVINCIBILITY)
						{
							dynamic_cast<CPlayer*>(pSrc)->SetHp(-1);
							dynamic_cast<CPlayer*>(pSrc)->SetInvinState(INVINCIBILITY);
						}
					}
					else if (pSrc->GetObjType() == PLAYER_BULLET)
					{
						dynamic_cast<CBullet*>(pSrc)->SetAniFlag(true);
					}
					if (pSrc->GetObjType() == ROCK)
					{
						dynamic_cast<CMonster*>(pDest)->Refresh();
						if (dynamic_cast<CMonster*>(pDest)->GetMonsterSRect().right < pSrc->GetRect().right)
						{
							tempP.x = -(rc.right - rc.left);
						}
						else
						{
							tempP.x = (rc.right - rc.left);
						}
						if (dynamic_cast<CMonster*>(pDest)->GetMonsterSRect().top < pSrc->GetRect().top)
						{
							tempP.y = -(rc.bottom - rc.top);
						}
						else
						{
							tempP.y = (rc.bottom - rc.top);
						}
						if (rc.bottom - rc.top > rc.right - rc.left)
							dynamic_cast<CMonster*>(pDest)->SetMonsterSRectP(dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().x + tempP.x, dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().y);
						else
							dynamic_cast<CMonster*>(pDest)->SetMonsterSRectP(dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().x, dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().y + tempP.y);

					}
				}
			}
		}
	}
}

void CCollisionMgr::CollisionWall(OBJECT_LIST & dstList, OBJECT_LIST &  srcList)
{
	for (auto pDest : dstList)
	{
		for (auto pSrc : srcList)
		{
			POINT tempP = {};
			RECT rc = {};
			RECT wall1 = { 0,0,0,0 };
			RECT wall2 = { 0,0,0,0 };
			RECT bwall1 = { 0,0,0,0 };
			RECT bwall2 = { 0,0,0,0 };

			for (int i = 0; i < 4; i++)
			{
				wall1 = dynamic_cast<CPlayer*>(pSrc)->GetNowRoomInfo()->GetfloorInfo(i).COL_1;
				wall2 = dynamic_cast<CPlayer*>(pSrc)->GetNowRoomInfo()->GetfloorInfo(i).COL_2;
				switch (i)
				{
				case 0:
					bwall1 = { wall1.left, wall1.top, wall1.right - 50, wall1.bottom };
					bwall2 = { wall2.left, wall2.top, wall2.right, wall2.bottom - 50 };
					break;
				case 1:
					bwall1 = { wall1.left + 50, wall1.top, wall1.right, wall1.bottom };
					bwall2 = { wall2.left, wall2.top, wall2.right, wall2.bottom - 50 };
					break;
				case 2:
					bwall1 = { wall1.left,wall1.top,wall1.right - 50,wall1.bottom };
					bwall2 = { wall2.left,wall2.top,wall2.right,wall2.bottom };
					break;
				case 3:
					bwall1 = { wall1.left + 50,wall1.top,wall1.right,wall1.bottom };
					bwall2 = { wall2.left,wall2.top,wall2.right,wall2.bottom };
					break;
				}
				if (IntersectRect(&rc, &pDest->GetRect(), &bwall1) || IntersectRect(&rc, &pDest->GetRect(), &bwall2))
				{
					if (pDest->GetObjType() == MONSTER_BULLET || pDest->GetObjType() == PLAYER_BULLET || pDest->GetObjType() == MONSTRO_BULLET || pDest->GetObjType() == BONY_BULLET)
					{
						dynamic_cast<CBullet*>(pDest)->SetAniFlag(true);
					}
				}
				if (IntersectRect(&rc, &pDest->GetRect(), &wall1))
				{

					if (pDest->GetObjType() == PLAYER || pDest->GetObjType() == MONSTER)
					{
						if (pDest->GetObjType() == MONSTER)
						{
							dynamic_cast<CMonster*>(pDest)->Refresh();
						}
						ColObject(pDest, wall1);
					}

				}
				if (IntersectRect(&rc, &pDest->GetRect(), &wall2))
				{

					if (pDest->GetObjType() == PLAYER || pDest->GetObjType() == MONSTER)
					{
						if (pDest->GetObjType() == MONSTER)
						{
							dynamic_cast<CMonster*>(pDest)->Refresh();
						}
						ColObject(pDest, wall2);
					}
				}

				if (pDest->GetObjType() == MONSTER)
				{
					if (IntersectRect(&rc, &dynamic_cast<CMonster*>(pDest)->GetMonsterSRect(), &wall1))
					{
						dynamic_cast<CMonster*>(pDest)->Refresh();
						if (dynamic_cast<CMonster*>(pDest)->GetMonsterSRect().right < wall1.right)
						{
							tempP.x = -(rc.right - rc.left);
						}
						else
						{
							tempP.x = (rc.right - rc.left);
						}
						if (dynamic_cast<CMonster*>(pDest)->GetMonsterSRect().top < wall1.top)
						{
							tempP.y = -(rc.bottom - rc.top);
						}
						else
						{
							tempP.y = (rc.bottom - rc.top);
						}
						if (rc.bottom - rc.top > rc.right - rc.left)
							dynamic_cast<CMonster*>(pDest)->SetMonsterSRectP(dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().x + tempP.x, dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().y);
						else
							dynamic_cast<CMonster*>(pDest)->SetMonsterSRectP(dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().x, dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().y + tempP.y);
					}

					if (IntersectRect(&rc, &dynamic_cast<CMonster*>(pDest)->GetMonsterSRect(), &wall2))
					{
						dynamic_cast<CMonster*>(pDest)->Refresh();
						if (dynamic_cast<CMonster*>(pDest)->GetMonsterSRect().right < wall2.right)
						{
							tempP.x = -(rc.right - rc.left);
						}
						else
						{
							tempP.x = (rc.right - rc.left);
						}
						if (dynamic_cast<CMonster*>(pDest)->GetMonsterSRect().top < wall2.top)
						{
							tempP.y = -(rc.bottom - rc.top);
						}
						else
						{
							tempP.y = (rc.bottom - rc.top);
						}
						if (rc.bottom - rc.top > rc.right - rc.left)
							dynamic_cast<CMonster*>(pDest)->SetMonsterSRectP(dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().x + tempP.x, dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().y);
						else
							dynamic_cast<CMonster*>(pDest)->SetMonsterSRectP(dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().x, dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().y + tempP.y);
					}
				}
			}
		}
	}
}
void CCollisionMgr::CollisionDoor(OBJECT_LIST & dstList, vector<CRoom*>& srcList)
{
	for (auto pDest : dstList)
	{
		POINT tempP = {};
		RECT rc = {};
		RECT door = { 0,0,0,0 };
		RECT doorBullet = { 0,0,0,0 };
		RECT tempdoorBullet = { 0,0,0,0 };
		for (auto pSrc : srcList)
		{
			for (int i = 0; i < 4; i++)
			{
				door = { 0,0,0,0 };
				door = pSrc->GetDoorCol(i);
				if (door.left == 0 && door.top == 0)
				{
					continue;
				}
				if (pSrc->GetRoom_State() == NO_BATTLE)
				{
					switch (i)
					{
					case LEFT:
						doorBullet = { pSrc->GetDoorCol(i).left ,pSrc->GetDoorCol(i).top,pSrc->GetDoorCol(i).right ,pSrc->GetDoorCol(i).bottom };
						break;
					case RIGHT:
						doorBullet = { pSrc->GetDoorCol(i).left  ,pSrc->GetDoorCol(i).top  ,pSrc->GetDoorCol(i).right , pSrc->GetDoorCol(i).bottom };
						break;
					case TOP:
						doorBullet = { pSrc->GetDoorCol(i).left ,pSrc->GetDoorCol(i).top ,pSrc->GetDoorCol(i).right ,pSrc->GetDoorCol(i).bottom };
						break;
					case BOTTOM:
						doorBullet = { pSrc->GetDoorCol(i).left ,pSrc->GetDoorCol(i).top - WALL_Y_SIZE * 1 / 4  ,pSrc->GetDoorCol(i).right  ,pSrc->GetDoorCol(i).bottom };
						break;
					}
				}
				else
				{
					switch (i)
					{
					case LEFT:
						doorBullet = { pSrc->GetDoorCol(i).left ,pSrc->GetDoorCol(i).top,pSrc->GetDoorCol(i).right - WALL_X_SIZE * 1 / 4 ,pSrc->GetDoorCol(i).bottom };
						break;
					case RIGHT:
						doorBullet = { pSrc->GetDoorCol(i).left + WALL_X_SIZE * 1 / 4  ,pSrc->GetDoorCol(i).top  ,pSrc->GetDoorCol(i).right , pSrc->GetDoorCol(i).bottom };
						break;
					case TOP:
						doorBullet = { pSrc->GetDoorCol(i).left ,pSrc->GetDoorCol(i).top ,pSrc->GetDoorCol(i).right ,pSrc->GetDoorCol(i).bottom - WALL_Y_SIZE * 1 / 4 };
						break;
					case BOTTOM:
						doorBullet = { pSrc->GetDoorCol(i).left ,pSrc->GetDoorCol(i).top  ,pSrc->GetDoorCol(i).right  ,pSrc->GetDoorCol(i).bottom };
						break;
					}
				}
				if (IntersectRect(&rc, &pDest->GetRect(), &doorBullet))
				{
					if (pDest->GetObjType() == MONSTER_BULLET || pDest->GetObjType() == PLAYER_BULLET || pDest->GetObjType() == MONSTRO_BULLET || pDest->GetObjType() == BONY_BULLET)
					{
						dynamic_cast<CBullet*>(pDest)->SetAniFlag(true);
					}
					else if (pDest->GetObjType() == MONSTER)
					{
						dynamic_cast<CMonster*>(pDest)->Refresh();

						if (pDest->GetRect().right < doorBullet.right)
						{
							tempP.x = -(rc.right - rc.left);
						}
						else
						{
							tempP.x = (rc.right - rc.left);
						}
						if (pDest->GetRect().top < doorBullet.top)
						{
							tempP.y = -(rc.bottom - rc.top);
						}
						else
						{
							tempP.y = (rc.bottom - rc.top);
						}
						if (rc.bottom - rc.top > rc.right - rc.left)
							pDest->SetPos(pDest->GetInfo().fX + tempP.x, pDest->GetInfo().fY);
						else
							pDest->SetPos(pDest->GetInfo().fX, pDest->GetInfo().fY + tempP.y);
					}
				}
				if (pDest->GetObjType() == MONSTER)
				{
					if (IntersectRect(&rc, &dynamic_cast<CMonster*>(pDest)->GetMonsterSRect(), &door))
					{
						dynamic_cast<CMonster*>(pDest)->Refresh();
						if (dynamic_cast<CMonster*>(pDest)->GetMonsterSRect().right < door.right)
						{
							tempP.x = -(rc.right - rc.left);
						}
						else
						{
							tempP.x = (rc.right - rc.left);
						}
						if (dynamic_cast<CMonster*>(pDest)->GetMonsterSRect().top < door.top)
						{
							tempP.y = -(rc.bottom - rc.top);
						}
						else
						{
							tempP.y = (rc.bottom - rc.top);
						}
						if (rc.bottom - rc.top > rc.right - rc.left)
							dynamic_cast<CMonster*>(pDest)->SetMonsterSRectP(dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().x + tempP.x, dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().y);
						else
							dynamic_cast<CMonster*>(pDest)->SetMonsterSRectP(dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().x, dynamic_cast<CMonster*>(pDest)->GetMonsterSRectP().y + tempP.y);
					}
				}
				if (IntersectRect(&rc, &pDest->GetRect(), &door))
				{
					if (pDest->GetObjType() == PLAYER && dynamic_cast<CPlayer*>(pDest)->GetMoveState() == NO_MOVE)
					{
						if (pSrc->GetRoom_State() == NOW_BATTLE)
						{
							if (pDest->GetRect().right < door.right)
							{
								tempP.x = -(rc.right - rc.left);
							}
							else
							{
								tempP.x = (rc.right - rc.left);
							}
							if (pDest->GetRect().top < door.top)
							{
								tempP.y = -(rc.bottom - rc.top);
							}
							else
							{
								tempP.y = (rc.bottom - rc.top);
							}
							if (rc.bottom - rc.top > rc.right - rc.left)
								pDest->SetPos(pDest->GetInfo().fX + tempP.x, pDest->GetInfo().fY);
							else
								pDest->SetPos(pDest->GetInfo().fX, pDest->GetInfo().fY + tempP.y);
						}
						else
						{
							dynamic_cast<CPlayer*>(pDest)->SetMoveState(MOVE_ROOM);
							CSceneMgr::GetInstance()->SetMapScroll(i);
						}
					}
					if (pDest->GetObjType() == MONSTER)
					{
						dynamic_cast<CMonster*>(pDest)->Refresh();
						ColObject(pDest, door);
					}

				}
			}
		}
	}
}


bool CCollisionMgr::IntersectSphere(CGameObject* pDest, CGameObject* pSource)
{
	// 원충돌
	// 두 원의 반지름 합을 구한다. r1 + r2
	// 두 원의 중점 간 거리를 구한다. d
	// r1 + r2 >= d 조건을 만족하면 TRUE

	float r1 = pDest->GetInfo().fCX * 0.5f;
	float r2 = pSource->GetInfo().fCX * 0.5f;

	float w = pDest->GetInfo().fX - pSource->GetInfo().fX;
	float h = pDest->GetInfo().fY - pSource->GetInfo().fY;

	// sqrtf: 제곱근 구하는 함수. <cmath>에서 제공.
	float d = sqrtf(w * w + h * h);

	return r1 + r2 >= d;
}

bool CCollisionMgr::IntersectRectEx(CGameObject* pDest, CGameObject* pSource, float* pMoveX, float* pMoveY)
{
	// x축의 반지름 합과 거리를 구한다.
	// 이 때 x축의 반지름 합이 x축의 거리보다 크면 x쪽으로 겹쳤다.
	float fSumX = pDest->GetInfo().fCX * 0.5f + pSource->GetInfo().fCX * 0.5f;
	float fDistX = fabs(pDest->GetInfo().fX - pSource->GetInfo().fX);

	// y축의 반지름 합과 거리를 구한다.
	// 이 때 y축의 반지름 합이 y축의 거리보다 크면 y쪽으로 겹쳤다.
	float fSumY = pDest->GetInfo().fCY * 0.5f + pSource->GetInfo().fCY * 0.5f;
	float fDistY = fabs(pDest->GetInfo().fY - pSource->GetInfo().fY);

	if (fSumX >= fDistX && fSumY >= fDistY)
	{
		// 겹친영역의 크기 구하기.
		*pMoveX = fSumX - fDistX;
		*pMoveY = fSumY - fDistY;

		return true;
	}

	return false;
}

void CCollisionMgr::ColObject(CGameObject * pDest, RECT pSource)
{
	RECT rc = {};
	POINT tempP = {};
	if (IntersectRect(&rc, &pDest->GetRect(), &pSource))
	{
		if (pDest->GetRect().right < pSource.right)
		{
			tempP.x = -(rc.right - rc.left);
		}
		else
		{
			tempP.x = (rc.right - rc.left);
		}
		if (pDest->GetRect().top < pSource.top)
		{
			tempP.y = -(rc.bottom - rc.top);
		}
		else
		{
			tempP.y = (rc.bottom - rc.top);
		}
		if (rc.bottom - rc.top > rc.right - rc.left)
			pDest->SetPos(pDest->GetInfo().fX + tempP.x, pDest->GetInfo().fY);
		else
			pDest->SetPos(pDest->GetInfo().fX, pDest->GetInfo().fY + tempP.y);
	}
}
