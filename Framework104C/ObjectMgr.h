#pragma once

// ������ ���� (Mediator Pattern)
// ��ȣ �������� ���� M : N ���踦 M : 1 ����� ���� �������� ���ߴ� ������ ����.
// ���α׷��� �����ϴ� ������Ʈ���� ���� ����� �� �� �����ڸ� ���ؼ� �����Ѵ�.
// ��, ������Ʈ���� ���θ� ���� �����ڸ� �˸� ����� �ȴ�.

class CGameObject;
class CRoom;
class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)
private: // �̱��� ��ü�� �ܺο��� ������ �Ҹ��� �� �� ����.
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
	// �̱��� ���� (Singleton Pattern)
	// ��ü ���� ���� �����ϴ� ������ ����.
	// �̱����� �̿��Ͽ� ������ ��ü�� �������� ������ �� �ִ� Ư���� ����.

};

