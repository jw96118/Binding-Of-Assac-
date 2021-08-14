#pragma once
class CTitle;
class CScene;
class CRoom;
class CStage;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)
private:
	CSceneMgr();
	~CSceneMgr();
public :
	void Initialize();
	void Update();
	void Render(HDC hDC);
	GAME_STATE GetGameState();
private:
	void Relese();
private:
	void KeyInput();
	void ScrollDown(GAME_STATE _state,int _y);
	void ScrollUp(GAME_STATE _state, int _y);
	void PlayerBGM(const wstring& wstrSoundKey);
	void StopBGM(SOUNDMGR::CHANNEL_ID eID);

public:
	 vector<CRoom*>& GetRoomInfo() ;
	 CScene* GetGrid() const;
	 const bool& GetCutScene();
	 void SetMapScroll(int i);
	 void SetCutScene(bool flag);
private:
	GAME_STATE gameState;
	GAME_STATE preGameState;


	vector<CScene*> sceneVec;
	CTitle* title;
	int scrollCount;
	bool scrollFlag;
	int mainX;
	int mainY;
	bool soundPlay = false;
};

