#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	: m_bIsDead(false),
	m_fSpeed(0.f),
	m_fAngle(0.f)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
}


CGameObject::~CGameObject()
{
}

const INFO& CGameObject::GetInfo() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_tInfo;
}

const RECT& CGameObject::GetRect() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_tRect;
}

const OBJECT_TYPE & CGameObject::GetObjType() const
{
	return type;
}

const bool & CGameObject::GetEnable() const
{
	return enable;
}

void CGameObject::SetPos(float x, float y)
{
	m_tInfo.fX = x;
	m_tInfo.fY = y;
}

void CGameObject::SetAngle(float degree)
{
	m_fAngle = degree;
}

void CGameObject::SetObjType(OBJECT_TYPE _type)
{
}

void CGameObject::IsDead()
{
	m_bIsDead = true;
}

void CGameObject::SetEnable(bool flag)
{
	enable = flag;
}

void CGameObject::UpdateRect()
{
	m_tRect.left = LONG(m_tInfo.fX - m_tInfo.fCX * 0.5f);
	m_tRect.top = LONG(m_tInfo.fY - m_tInfo.fCY * 0.5f);
	m_tRect.right = LONG(m_tInfo.fX + m_tInfo.fCX * 0.5f);
	m_tRect.bottom = LONG(m_tInfo.fY + m_tInfo.fCY * 0.5f);
}

void CGameObject::PlayerBGM(const wstring & wstrSoundKey)
{
	if (!soundPlay)
	{
		CSoundMgr::GetInstance()->PlayBGM(wstrSoundKey);
		soundPlay = true;
	}
}

void CGameObject::PlayerSound(const wstring & wstrSoundKey, SOUNDMGR::CHANNEL_ID eID)
{
	if (!soundPlay)
	{
		CSoundMgr::GetInstance()->PlaySound(wstrSoundKey, eID);
		soundPlay = true;
	}
}

void CGameObject::StopBGM(SOUNDMGR::CHANNEL_ID eID)
{
	CSoundMgr::GetInstance()->StopSound(eID);
	soundPlay = false;
}
