#pragma once
class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void Render(HDC hDC) = 0;

protected:
	virtual void Release() = 0;
	void PlayerBGM(const wstring& wstrSoundKey);
	void PlayerSound(const wstring& wstrSoundKey, SOUNDMGR::CHANNEL_ID eID);
	void StopBGM(SOUNDMGR::CHANNEL_ID eID);
public :
	void SetRenderPt(int x, int y);
	
protected:
	DWORD dwOldTime = GetTickCount();
	DWORD dwCurTime = 0;
	int count;
	Image* image;

	float renderX;
	float renderY;


	HDC hMemDC;

	bool soundPlay = false;
};

