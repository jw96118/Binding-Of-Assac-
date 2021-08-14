#pragma once
#include "Scene.h"
class CRoom;
class CStage :
	public CScene
{
public:
	CStage();
	~CStage();
	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;

	virtual int Update() override;

	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;
public:
	 vector<CRoom*>& GetRoomInfo() ;
	 const bool& GetCutScene();
	 void SetScroll(int i);
	 void SetCutScene(bool flag);
	 
private:
	bool MakeRandomRoom();
	void ScrollRoom();
	void MonsterCheck();
	void SettingMonster();
	void BossCutScene(HDC hDC);
	void MakeItem();
private:
	vector<CRoom*> roomInfo;
	POINT renderP;
	POINT scroll;

	POINT pCut;
	POINT mCut; 
	POINT pCutName;
	POINT mCutName;
	POINT cutVs;
	bool startCut;
	unsigned int cutCount;
	ROOM_TYPE roomT;
	ROOM_DIRECTION scrollRoom;
	CGameObject* playerInfo;
	bool scrollFlag;
	bool makeBossRoomflag;
	bool makeGoldRoomflag;
	int scrollCheck[4];


	bool CutSound = false;
};

