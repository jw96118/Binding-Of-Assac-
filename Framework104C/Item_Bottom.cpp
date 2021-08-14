#include "stdafx.h"
#include "Item_Bottom.h"


CItem_Bottom::CItem_Bottom()
{
}


CItem_Bottom::~CItem_Bottom()
{
}

void CItem_Bottom::Initialize()
{

	CImageMgr::GetInstance()->AddImage(L"ITEM_BOTTOM", L"Image/item/levelitem_001_itemaltar.png", 32, 32);
	hMemDC = CImageMgr::GetInstance()->GetDc(L"ITEM_BOTTOM");

	imageSize.x = 80;
	imageSize.y = 80;

	m_tInfo.fCX = 60.f;
	m_tInfo.fCY = 50.f;

	aniWidth = 0;
	aniHeight = 0;

	dwOldTime_Render = GetTickCount();
	dwCurTime_Render = 0;

	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;
	type = ITEM_BOTTOM;
}

int CItem_Bottom::Update()
{
	CGameObject::UpdateRect();
	CObjectMgr::GetInstance()->AddLayer(LAYER_OBJECT, this);

	return 0;
}

void CItem_Bottom::Render(HDC hDC)
{
	renderX = (int)m_tInfo.fX - imageSize.x / 2 + (int)g_fScrollX;
	renderY = (int)m_tInfo.fY - imageSize.y / 2 + (int)g_fScrollY;
	GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CItem_Bottom::Release()
{
}
