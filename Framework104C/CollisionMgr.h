#pragma once
class CRoom;
class CCollisionMgr
{
private:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void CollisionRect(OBJECT_LIST& dstList, OBJECT_LIST& srcList);
	static void CollisionRectEx(OBJECT_LIST& dstList, OBJECT_LIST& srcList);
	static void CollisionSphere(OBJECT_LIST& dstList, OBJECT_LIST& srcList);
	static void CollisionObject(OBJECT_LIST& dstList, OBJECT_LIST& srcList);
	static void CollisionWall(OBJECT_LIST& dstList, OBJECT_LIST& srcList);
	static void CollisionDoor(OBJECT_LIST& dstList, vector<CRoom*>& srcList);
private:
	static bool IntersectSphere(CGameObject* pDest, CGameObject* pSource);
	static bool IntersectRectEx(CGameObject* pDest, CGameObject* pSource, float* pMoveX, float* pMoveY);
	static void ColObject(CGameObject* pDest, RECT pSource);
};

