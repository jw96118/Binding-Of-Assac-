#pragma once

#ifndef __OBJECT_FACTORY_H__

class CGameObject;

/* 추상 팩토리 패턴 */
// 객체 생성 통일 및 캡슐화에 초점을 둔 디자인 패턴

template <typename T>
class CObjectFactory 
{
public:
	static CGameObject* CreateObject()
	{
		CGameObject* pObject = new T;
		pObject->Initialize();	

		pObject->SetEnable(true);
		return pObject;
	}

	static CGameObject* CreateObject(float x, float y)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		pObject->SetPos(x, y);

		pObject->SetEnable(true);
		return pObject;
	}

	static CGameObject* CreateObject(float x, float y, float degree)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		pObject->SetPos(x, y);
		pObject->SetAngle(degree);

		pObject->SetEnable(true);
		return pObject;
	}
	static CGameObject* CreateObject(float x, float y, float movex, float movey,float range)
	{
		CGameObject* pObject = new T;
		pObject->SetPos(x, y);

		dynamic_cast<CBullet*>(pObject)->SetMovePos(movex, movey);
		dynamic_cast<CBullet*>(pObject)->SetRange(range);
		pObject->Initialize();
		pObject->SetEnable(true);
	
		return pObject;
	}
	static CGameObject* CreateObject(float x, float y, float movex, float movey, float range , POINT pt,float colx,float coly, wstring image)
	{
		CGameObject* pObject = new T;
		pObject->SetPos(x, y);

		dynamic_cast<CBullet*>(pObject)->SetMovePos(movex, movey);
		dynamic_cast<CBullet*>(pObject)->SetRange(range);
		dynamic_cast<CPlayerBullet*>(pObject)->SetBulletType(image);
		dynamic_cast<CPlayerBullet*>(pObject)->SetColSize(colx, coly);
		dynamic_cast<CPlayerBullet*>(pObject)->SetImageSize(pt);
		pObject->Initialize();
		pObject->SetEnable(true);

		return pObject;
	}

	static CGameObject* CreateObject(float x, float y, MONSTER_TYPE type)
	{
		CGameObject* pObject = new T;
		pObject->SetPos(x, y);
		pObject->Initialize();
		
		pObject->SetEnable(true);
		dynamic_cast<CMonster*>(pObject)->SetMonsterType(type);
		return pObject;
	}

	static CGameObject* CreateObject(float x, float y, P_ITEM_LIST type)
	{
		CGameObject* pObject = new T;
		pObject->SetPos(x, y);
		pObject->Initialize();

		pObject->SetEnable(true);
		dynamic_cast<CP_Item*>(pObject)->SetItemType(type);

		return pObject;
	}
};

#define __OBJECT_FACTORY_H__
#endif
