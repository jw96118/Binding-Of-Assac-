#include "stdafx.h"
#include "ObjectMgr.h"
#include "GameObject.h"
#include "Stage.h"
#include "Player.h"
#include "Room.h"
#include "Monster.h"
#include "PlayerBullet.h"
#include "BonyBullet.h"
#include "MonsterBullet.h"
#include "MonstroBullet_1.h"
IMPLEMENT_SINGLETON(CObjectMgr)
CObjectMgr::CObjectMgr()
{
}


CObjectMgr::~CObjectMgr()
{
	Release();
}

CGameObject* CObjectMgr::GetPlayer() const
{
	if (m_ObjectList[PLAYER].empty())
		return nullptr;

	return m_ObjectList[PLAYER].front();
}

CGameObject * CObjectMgr::GetMiniMap() const
{
	if (m_ObjectList[UI].empty())
		return nullptr;

	return m_ObjectList[UI].front();
}

CGameObject * CObjectMgr::GetBoss() const
{
	if (m_ObjectList[MONSTER].empty())
		return nullptr;

	auto iter_begin = m_ObjectList[MONSTER].begin();
	auto iter_end = m_ObjectList[MONSTER].end();


	for (; iter_begin != iter_end; iter_begin++)
	{
		if (dynamic_cast<CMonster*>(*iter_begin)->GetMonsterType() == MONSTRO)
		{
			return *iter_begin;
		}
	}
	return nullptr;

}

CGameObject* CObjectMgr::GetNearTarget(CGameObject* pObject, OBJECT_TYPE eType)
{
	if (m_ObjectList[eType].empty())
		return nullptr;

	CGameObject* pNear = m_ObjectList[eType].front();

	float w = pObject->GetInfo().fX - pNear->GetInfo().fX;
	float h = pObject->GetInfo().fY - pNear->GetInfo().fY;
	float fNearDist = sqrtf(w * w + h * h);

	for (auto pTarget : m_ObjectList[eType])
	{
		w = pObject->GetInfo().fX - pTarget->GetInfo().fX;
		h = pObject->GetInfo().fY - pTarget->GetInfo().fY;
		float d = sqrtf(w * w + h * h);

		if (fNearDist > d)
		{
			pNear = pTarget;
			fNearDist = d;
		}
	}

	return pNear;
}

OBJECT_LIST CObjectMgr::GetMonster() const
{
	if (m_ObjectList[MONSTER].empty())
	{
	}
	return m_ObjectList[MONSTER];
}

CGameObject * CObjectMgr::GetShield1() const
{
	if (m_ObjectList[SHIELD].empty())
	{
		return nullptr;
	}
	return m_ObjectList[SHIELD].front();
}

CGameObject * CObjectMgr::GetShield2() const
{
	if (m_ObjectList[SHIELD].empty())
	{
		return nullptr;
	}
	return m_ObjectList[SHIELD].back();

}


void CObjectMgr::SetPlayerRoom(CRoom* room)
{
	if (m_ObjectList[PLAYER].empty())
		return;

	dynamic_cast<CPlayer*>(m_ObjectList[PLAYER].front())->SetNowRoomInfo(room);
}

void CObjectMgr::SetPlayerMoveState(PLAYER_MOVE_STATE state)
{
	if (m_ObjectList[PLAYER].empty())
		return;

	dynamic_cast<CPlayer*>(m_ObjectList[PLAYER].front())->SetMoveState(state);
}

void CObjectMgr::SetPlayerMoveDirection(ROOM_DIRECTION dir)
{
	if (m_ObjectList[PLAYER].empty())
		return;

	dynamic_cast<CPlayer*>(m_ObjectList[PLAYER].front())->SetScrollDir(dir);
}

void CObjectMgr::AddObject(OBJECT_TYPE eType, CGameObject* pObject)
{
	if (eType == MONSTER_BULLET || eType == PLAYER_BULLET)
	{
		bool insertflag = true;
		if (!m_ObjectList[eType].empty())
		{
			auto iter_begin = m_ObjectList[eType].begin();
			auto iter_end = m_ObjectList[eType].end();
			for (; iter_begin != iter_end; iter_begin++)
			{
				if (!(*iter_begin)->GetEnable())
				{
					insertflag = false;
					(*iter_begin) = pObject;
					break;
				}
			}
		}
		if (insertflag)
		{
			m_ObjectList[eType].push_back(pObject);
		}
	}
	else
		m_ObjectList[eType].push_back(pObject);
}
void CObjectMgr::AddPlayerBullet(OBJECT_TYPE eType, float x, float y, float movex, float movey, float range, POINT pt, float colx, float coly, wstring image)
{
	if (eType == PLAYER_BULLET)
	{
		bool insertflag = true;
		if (!m_ObjectList[eType].empty())
		{
			auto iter_begin = m_ObjectList[eType].begin();
			auto iter_end = m_ObjectList[eType].end();
			for (; iter_begin != iter_end; iter_begin++)
			{
				if (!(*iter_begin)->GetEnable())
				{
					insertflag = false;
					(*iter_begin)->SetPos(x, y);
					dynamic_cast<CBullet*>((*iter_begin))->SetMovePos(movex, movey);
					dynamic_cast<CBullet*>((*iter_begin))->SetRange(range);
					dynamic_cast<CPlayerBullet*>((*iter_begin))->SetBulletType(image);
					dynamic_cast<CPlayerBullet*>((*iter_begin))->SetColSize(colx, coly);
					dynamic_cast<CPlayerBullet*>((*iter_begin))->SetImageSize(pt);
					(*iter_begin)->Initialize();
					(*iter_begin)->SetEnable(true);

					break;
				}
			}
		}
		if (insertflag)
		{
			m_ObjectList[eType].push_back(CObjectFactory<CPlayerBullet>::CreateObject(x, y, movex, movey, range, pt, colx, coly, image));
		}
	}
	else
	{
		return;
	}
}
void CObjectMgr::AddMonsterBullet(OBJECT_TYPE eType, float x, float y, float movex, float movey, float range)
{
	if (eType == MONSTER_BULLET)
	{
		bool insertflag = true;
		if (!m_ObjectList[eType].empty())
		{
			auto iter_begin = m_ObjectList[eType].begin();
			auto iter_end = m_ObjectList[eType].end();
			for (; iter_begin != iter_end; iter_begin++)
			{
				if (!(*iter_begin)->GetEnable())
				{
					insertflag = false;
					(*iter_begin)->SetPos(x, y);
					dynamic_cast<CBullet*>((*iter_begin))->SetMovePos(movex, movey);
					dynamic_cast<CBullet*>((*iter_begin))->SetRange(range);
					(*iter_begin)->Initialize();
					(*iter_begin)->SetEnable(true);
					break;
				}
			}
		}
		if (insertflag)
		{


			m_ObjectList[eType].push_back(CObjectFactory<CMonsterBullet>::CreateObject(x, y, movex, movey, range));

		}
	}
	else
	{
		return;
	}
}
void CObjectMgr::AddMonstroBullet(OBJECT_TYPE eType, float x, float y, float movex, float movey, float range)
{
	if (eType == MONSTRO_BULLET)
	{
		bool insertflag = true;
		if (!m_ObjectList[eType].empty())
		{
			auto iter_begin = m_ObjectList[eType].begin();
			auto iter_end = m_ObjectList[eType].end();
			for (; iter_begin != iter_end; iter_begin++)
			{
				if (!(*iter_begin)->GetEnable())
				{
					insertflag = false;
					(*iter_begin)->SetPos(x, y);
					dynamic_cast<CBullet*>((*iter_begin))->SetMovePos(movex, movey);
					dynamic_cast<CBullet*>((*iter_begin))->SetRange(range);
					(*iter_begin)->Initialize();
					(*iter_begin)->SetEnable(true);
					break;
				}
			}
		}
		if (insertflag)
		{

			m_ObjectList[eType].push_back(CObjectFactory<CMonstroBullet_1>::CreateObject(x, y, movex, movey, range));

		}
	}
	else
	{
		return;
	}
}
void CObjectMgr::AddBoneyBullet(OBJECT_TYPE eType, float x, float y, float movex, float movey, float range)
{
	if (eType == BONY_BULLET)
	{
		bool insertflag = true;
		if (!m_ObjectList[eType].empty())
		{
			auto iter_begin = m_ObjectList[eType].begin();
			auto iter_end = m_ObjectList[eType].end();
			for (; iter_begin != iter_end; iter_begin++)
			{
				if (!(*iter_begin)->GetEnable())
				{
					insertflag = false;
					(*iter_begin)->SetPos(x, y);
					dynamic_cast<CBullet*>((*iter_begin))->SetMovePos(movex, movey);
					dynamic_cast<CBullet*>((*iter_begin))->SetRange(range);
					(*iter_begin)->Initialize();
					(*iter_begin)->SetEnable(true);
					break;
				}
			}
		}
		if (insertflag)
		{

			m_ObjectList[eType].push_back(CObjectFactory<CBonyBullet>::CreateObject(x, y, movex, movey, range));

		}
	}
	else
	{
		return;
	}
}
void CObjectMgr::AddLayer(RENDER_LAYER eLayer, CGameObject* pObject)
{
	m_RenderList[eLayer].push_back(pObject);
}
void CObjectMgr::Update()
{
	// 이터레이터 패턴(반복자 패턴)
	// 객체 관리 통일성에 초점을 둔 디자인 패턴

	for (int i = 0; i < OBJECT_END; ++i)
	{
		if (i != MONSTER)
		{
			auto iter_begin = m_ObjectList[i].begin();
			auto iter_end = m_ObjectList[i].end();

			for (; iter_begin != iter_end; )
			{

				int iEvent = NO_EVENT;
				if ((*iter_begin)->GetEnable())
					iEvent = (*iter_begin)->Update();

				if (DEAD_OBJ == iEvent)
				{
					(*iter_begin)->SetEnable(false);
					//SafeDelete((*iter_begin));
					//iter_begin = m_ObjectList[i].erase(iter_begin);
				}
				else
					++iter_begin;
			}
		}
	}

	//CCollisionMgr::CollisionRect(m_ObjectList[BULLET], m_ObjectList[MONSTER]);
	//CCollisionMgr::CollisionRect(m_ObjectList[BULLET], m_ObjectList[MONSTER]);
	//CCollisionMgr::CollisionSphere(m_ObjectList[SHIELD], m_ObjectList[MONSTER]);

	CCollisionMgr::CollisionWall(m_ObjectList[MONSTER_BULLET], m_ObjectList[PLAYER]);
	CCollisionMgr::CollisionWall(m_ObjectList[MONSTRO_BULLET], m_ObjectList[PLAYER]);
	CCollisionMgr::CollisionWall(m_ObjectList[BONY_BULLET], m_ObjectList[PLAYER]);
	CCollisionMgr::CollisionWall(m_ObjectList[PLAYER_BULLET], m_ObjectList[PLAYER]);
	CCollisionMgr::CollisionWall(m_ObjectList[PLAYER], m_ObjectList[PLAYER]);

	CCollisionMgr::CollisionDoor(m_ObjectList[MONSTER_BULLET], CSceneMgr::GetInstance()->GetRoomInfo());
	CCollisionMgr::CollisionDoor(m_ObjectList[PLAYER_BULLET], CSceneMgr::GetInstance()->GetRoomInfo());
	CCollisionMgr::CollisionDoor(m_ObjectList[PLAYER], CSceneMgr::GetInstance()->GetRoomInfo());
	CCollisionMgr::CollisionObject(m_ObjectList[PLAYER], m_ObjectList[P_ITEM]);
	CCollisionMgr::CollisionObject(m_ObjectList[PLAYER], m_ObjectList[ITEM_BOTTOM]);
	CCollisionMgr::CollisionObject(m_ObjectList[PLAYER], m_ObjectList[ROCK]);
	CCollisionMgr::CollisionObject(m_ObjectList[PLAYER_BULLET], m_ObjectList[ROCK]);
	CCollisionMgr::CollisionObject(m_ObjectList[PLAYER_BULLET], m_ObjectList[ITEM_BOTTOM]);
	CCollisionMgr::CollisionObject(m_ObjectList[BOMB], m_ObjectList[PLAYER]);
	CCollisionMgr::CollisionObject(m_ObjectList[BOMB], m_ObjectList[ROCK]);
	CCollisionMgr::CollisionObject(m_ObjectList[BOMB], m_ObjectList[MONSTER]);
	CCollisionMgr::CollisionObject(m_ObjectList[SHIELD], m_ObjectList[MONSTER_BULLET]);
	CCollisionMgr::CollisionObject(m_ObjectList[SHIELD], m_ObjectList[MONSTRO_BULLET]);
	CCollisionMgr::CollisionObject(m_ObjectList[SHIELD], m_ObjectList[BONY_BULLET]);
	CCollisionMgr::CollisionObject(m_ObjectList[EXPLOSION], m_ObjectList[MONSTER]);

}

void CObjectMgr::Render(CRoom* roomInfo, HDC hDC)
{
	if (!CSceneMgr::GetInstance()->GetCutScene())
	{
		// 이터레이터 패턴(반복자 패턴)
		for (int i = 0; i < LAYER_END; ++i)
		{

			m_RenderList[i].sort(CSortObjectY());

			auto iter_begin = m_RenderList[i].begin();
			auto iter_end = m_RenderList[i].end();

			for (; iter_begin != iter_end; ++iter_begin)
				if ((*iter_begin) != nullptr)
				{
					(*iter_begin)->Render(hDC);
				}
			m_RenderList[i].clear();
		}
	}


}

void CObjectMgr::MonsterUpdate(CRoom* roomInfo)
{
	int monsterCount = 0;
	int checkCount = 0;
	auto iter_begin = m_ObjectList[MONSTER].begin();
	auto iter_end = m_ObjectList[MONSTER].end();

	if (!scroll || !CSceneMgr::GetInstance()->GetCutScene())
	{
		for (; iter_begin != iter_end; )
		{
			if (((*iter_begin)->GetInfo().fX > roomInfo->GetRenderP().x && (*iter_begin)->GetInfo().fX < roomInfo->GetRenderP().x + WINCX
				&& (*iter_begin)->GetInfo().fY > roomInfo->GetRenderP().y && (*iter_begin)->GetInfo().fY < roomInfo->GetRenderP().y + WINCY)
				|| dynamic_cast<CMonster*>(*iter_begin)->GetUpdateStartCheck())
			{
				int iEvent = (*iter_begin)->Update();
				if (dynamic_cast<CMonster*>(*iter_begin)->GetState() != MONSTER_DEAD)
				{
					monsterCount++;
					//if (roomInfo->GetRoom_Type() == BOSS_ROOM)
						//CSceneMgr::GetInstance()->SetCutScene(true);
				}
				if (DEAD_OBJ == iEvent)
				{
					if (dynamic_cast<CMonster*>(*iter_begin)->GetMonsterType() == MONSTRO)
					{
						bossCheck = true;
					}
					SafeDelete((*iter_begin));
					iter_begin = m_ObjectList[MONSTER].erase(iter_begin);
				}
				else
					++iter_begin;
			}
			else
			{
				++iter_begin;
				roomInfo->SetRoomState(NO_BATTLE);
			}
		}
		if (monsterCount > 0)
		{
			roomInfo->SetRoomState(NOW_BATTLE);
		}
		else
		{
			roomInfo->SetRoomState(NO_BATTLE);
		}
		monsterCount = 0;
		CCollisionMgr::CollisionObject(m_ObjectList[MONSTER], m_ObjectList[PLAYER]);
		CCollisionMgr::CollisionObject(m_ObjectList[MONSTER], m_ObjectList[PLAYER_BULLET]);
		CCollisionMgr::CollisionObject(m_ObjectList[MONSTER_BULLET], m_ObjectList[PLAYER]);
		CCollisionMgr::CollisionObject(m_ObjectList[MONSTRO_BULLET], m_ObjectList[PLAYER]);
		CCollisionMgr::CollisionObject(m_ObjectList[BONY_BULLET], m_ObjectList[PLAYER]);
		CCollisionMgr::CollisionWall(m_ObjectList[MONSTER], m_ObjectList[PLAYER]);
		CCollisionMgr::CollisionRectEx(m_ObjectList[PLAYER_BULLET], m_ObjectList[MONSTER]);
		CCollisionMgr::CollisionDoor(m_ObjectList[MONSTER], CSceneMgr::GetInstance()->GetRoomInfo());
		CCollisionMgr::CollisionDoor(m_ObjectList[MONSTER_BULLET], CSceneMgr::GetInstance()->GetRoomInfo());
		CCollisionMgr::CollisionDoor(m_ObjectList[MONSTRO_BULLET], CSceneMgr::GetInstance()->GetRoomInfo());
		CCollisionMgr::CollisionDoor(m_ObjectList[BONY_BULLET], CSceneMgr::GetInstance()->GetRoomInfo());
		CCollisionMgr::CollisionObject(m_ObjectList[MONSTER], m_ObjectList[ROCK]);
		CCollisionMgr::CollisionObject(m_ObjectList[MONSTER_BULLET], m_ObjectList[ROCK]);
		CCollisionMgr::CollisionObject(m_ObjectList[MONSTRO_BULLET], m_ObjectList[ROCK]);
		CCollisionMgr::CollisionObject(m_ObjectList[BONY_BULLET], m_ObjectList[ROCK]);
	

	}
}

void CObjectMgr::MonsterRender(CRoom * roomInfo, HDC hDC)
{
	auto iter_begin = m_ObjectList[MONSTER].begin();
	auto iter_end = m_ObjectList[MONSTER].end();
	if (!CSceneMgr::GetInstance()->GetCutScene())
	{
		for (; iter_begin != iter_end; ++iter_begin)
		{
			if ((*iter_begin)->GetInfo().fX > roomInfo->GetRenderP().x && (*iter_begin)->GetInfo().fX < roomInfo->GetRenderP().x + WINCX
				&& (*iter_begin)->GetInfo().fY > roomInfo->GetRenderP().y && (*iter_begin)->GetInfo().fY < roomInfo->GetRenderP().y + WINCY
				|| dynamic_cast<CMonster*>(*iter_begin)->GetUpdateStartCheck())
			{
				(*iter_begin)->Render(hDC);
			}
		}
	}
}

void CObjectMgr::SetScroll(bool flag)
{
	scroll = flag;
}

bool CObjectMgr::BossCheck()
{
	return bossCheck;
}

void CObjectMgr::Release()
{
	for (int i = 0; i < OBJECT_END; ++i)
	{
		for_each(m_ObjectList[i].begin(), m_ObjectList[i].end(), SafeDelete<CGameObject*>);
		m_ObjectList[i].clear();
	}
	for (int i = 0; i < LAYER_END; ++i)
		m_RenderList[i].clear();
}


