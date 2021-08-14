#pragma once

// 중재자 패턴 (Mediator Pattern)
// 상호 의존도가 높은 M : N 관계를 M : 1 관계로 만들어서 의존도를 낮추는 디자인 패턴.
// 프로그램을 구성하는 오브젝트들은 서로 통신을 할 때 중재자를 통해서 수행한다.
// 즉, 오브젝트들은 서로를 몰라도 중재자만 알면 통신이 된다.

class CGameObject;
class CRoom;
class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)
private: // 싱글톤 객체는 외부에서 생성과 소멸을 할 수 없다.
	CObjectMgr();
	~CObjectMgr();

public:
	CGameObject* GetPlayer() const; 
	CGameObject* GetMiniMap() const;
	CGameObject* GetBoss() const;
	CGameObject* GetNearTarget(CGameObject* pObject, OBJECT_TYPE eType);
	OBJECT_LIST GetMonster() const;
	CGameObject* GetShield1() const;
	CGameObject* GetShield2() const;
	void SetPlayerRoom(CRoom* room);
	void SetPlayerMoveState(PLAYER_MOVE_STATE state);
	void SetPlayerMoveDirection(ROOM_DIRECTION dir);
public:
	void AddLayer(RENDER_LAYER eLayer, CGameObject* pObject);
	void AddObject(OBJECT_TYPE eType, CGameObject* pObject);
	void AddPlayerBullet(OBJECT_TYPE eType, float x, float y, float movex, float movey, float range, POINT pt, float colx, float coly, wstring image);
	void AddMonsterBullet(OBJECT_TYPE eType,  float x, float y, float movex, float movey, float range);
	void AddMonstroBullet(OBJECT_TYPE eType,  float x, float y, float movex, float movey, float range);
	void AddBoneyBullet(OBJECT_TYPE eType, float x, float y, float movex, float movey, float range);
	void Update();
	void Render(CRoom* roomInfo,HDC hDC);
	void MonsterUpdate(CRoom* roomInfo);
	void MonsterRender(CRoom* roomInfo, HDC hDC);
	void SetScroll(bool flag);
	bool BossCheck();
private:
	void Release();

private:
	OBJECT_LIST		m_ObjectList[OBJECT_END];

	OBJECT_LIST		m_RenderList[LAYER_END];
	bool scroll = false;
	bool bossCheck = false;
	// 싱글톤 패턴 (Singleton Pattern)
	// 객체 생성 수를 제한하는 디자인 패턴.
	// 싱글톤을 이용하여 생성된 객체는 전역으로 접근할 수 있는 특성을 가짐.

};

