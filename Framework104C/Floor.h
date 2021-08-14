#pragma once
#include "GameObject.h"
class CFloor :
	public CGameObject
{
public:
	CFloor();
	~CFloor();

	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;


public :
	void SetTile(const wstring& _tile);
	void SetPos(float _renderX, float _renderY,int _reveceX, int _reveceY,RECT rect1, RECT rect2,POINT scr);
	FCol GetCol();
	
private:
	void UpdateCol();
private:
	wstring tile;
	float renderX;
	float renderY;
	POINT testp;
	POINT scroll;
	int reverceX;
	int reverceY;
	FCol floorCol;
	FCol firstCol;
public:
	RECT trect;
};

