#pragma once

#ifndef __GAMEOBJECT_H__
class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

public:
	const INFO& GetInfo() const;
	const RECT& GetRect() const;
	const OBJECT_TYPE& GetObjType() const;
	const bool& GetEnable() const;
public:
	void SetPos(float x, float y);
	void SetAngle(float degree);
	void SetObjType(OBJECT_TYPE _type);
	void SetEnable(bool flag);

public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void Render(HDC hDC) = 0;

protected:
	virtual void Release() = 0;	

public:
	void IsDead();
protected:
	void UpdateRect();
	void PlayerBGM(const wstring& wstrSoundKey);
	void PlayerSound(const wstring& wstrSoundKey , SOUNDMGR::CHANNEL_ID eID);
	void StopBGM(SOUNDMGR::CHANNEL_ID eID);
protected:
	INFO	m_tInfo;
	RECT	m_tRect;
	OBJECT_TYPE type;

	bool	m_bIsDead;
	float	m_fSpeed;
	float	m_fAngle;

	Image* image;

	float aniWidth;
	float aniHeight;

	DWORD dwOldTime_Update = GetTickCount();
	DWORD dwCurTime_Update = 0;
	DWORD dwOldTime_Render = GetTickCount();
	DWORD dwCurTime_Render = 0;
	POINT imageSize;

	bool enable;
	HDC hMemDC;

	bool soundPlay = false;
};

#define __GAMEOBJECT_H__
#endif

