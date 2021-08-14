#pragma once

#ifndef __BULLET_H__

#include "GameObject.h"
class CBullet :	public CGameObject
{
public:
	CBullet();
	virtual ~CBullet();

public:
	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;

protected:
	virtual void Release() override;
public :
	void SetMovePos(float x, float y);
	void SetRange(float _range);
	void SetAniFlag(bool flag);
protected:
	float   range;
	float   moveX;
	float   moveY;
	float   firstX;
	float   firstY;
	int AnimationCount;
	bool aniFlag;

};

#define __BULLET_H__
#endif
