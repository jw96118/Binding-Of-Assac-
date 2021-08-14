#include "stdafx.h"
#include "P_Item.h"
#include "Player.h"
#include "Shield.h"
CP_Item::CP_Item()
{
}
CP_Item::~CP_Item()
{
}

void CP_Item::Initialize()
{
	CImageMgr::GetInstance()->AddImage(L"I_SPOONBENDER", L"Image/item/collectibles/collectibles_003_spoonbender.png", 32, 32);
	CImageMgr::GetInstance()->AddImage(L"I_CRICKETSHEAD", L"Image/item/collectibles/collectibles_004_cricketshead.png", 32, 32);
	CImageMgr::GetInstance()->AddImage(L"I_HALOOFFIES", L"Image/item/collectibles/collectibles_010_haloofflies.png", 32, 32);
	CImageMgr::GetInstance()->AddImage(L"I_THEPACT", L"Image/item/collectibles/collectibles_080_thepact.png", 32, 32);
	CImageMgr::GetInstance()->AddImage(L"I_THEMULIGAN", L"Image/item/collectibles/collectibles_151_themulligan.png", 32, 32);
	CImageMgr::GetInstance()->AddImage(L"I_FATE", L"Image/item/collectibles/collectibles_179_fate.png", 32, 32);
	CImageMgr::GetInstance()->AddImage(L"I_LOSTCONTACT", L"Image/item/collectibles/collectibles_213_lostcontact.png", 32, 32);
	CImageMgr::GetInstance()->AddImage(L"I_TWOTWO", L"Image/item/collectibles/collectibles_245_2020.png", 32, 32);

	imageSize.x = 80;
	imageSize.y = 80;

	m_tInfo.fCX = 70.f;
	m_tInfo.fCY = 60.f;

	aniWidth = 0;
	aniHeight = 0;

	dwOldTime_Render = GetTickCount();
	dwCurTime_Render = 0;

	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;
	updownFlag = false;
	colPlayer = false;
	type = P_ITEM;
}

int CP_Item::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;
	
	switch (itemType)
	{
	case NON:
		break;
	case SPOONBENDER:
		itemRender = L"I_SPOONBENDER";
		break;
	case CRICKETSHEAD:
		itemRender = L"I_CRICKETSHEAD";
		break;
	case HALOOFFIES:
		itemRender = L"I_HALOOFFIES";
		break;
	case THEPACT:
		itemRender = L"I_THEPACT";
		break;
	case THEMULIGAN:
		itemRender = L"I_THEMULIGAN";
		break;
	case FATE:
		itemRender = L"I_FATE";
		break;
	case LOSTCONTACT:
		itemRender = L"I_LOSTCONTACT";
		break;
	case TWOTWO:
		itemRender = L"I_TWOTWO";
		break;
	}
	hMemDC = CImageMgr::GetInstance()->GetDc(itemRender);
	CObjectMgr::GetInstance()->AddLayer(LAYER_ITEM, this);
	m_pTarget = CObjectMgr::GetInstance()->GetPlayer();
	if (!colPlayer)
	{
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 40)
		{
			if (!updownFlag)
			{
				m_tInfo.fY += 1.f;
				imageSize.x += 1.f;
			}
			else
			{
				m_tInfo.fY -= 1.f;
				imageSize.x -= 1.f;
			}
			count++;
			if (count % 10 == 0 && count != 0)
			{
				updownFlag = !updownFlag;
			}
			dwOldTime_Render = dwCurTime_Render;
		}
	}
	else
	{
		m_tInfo.fX = m_pTarget->GetInfo().fX;
		m_tInfo.fY = m_pTarget->GetInfo().fY -65;
		m_tInfo.fCX = 0.f;
		m_tInfo.fCY = 0.f;
		if (!dynamic_cast<CPlayer*>(m_pTarget)->GetHaveItem())
		{
			imageSize = { 0,0 };
		}
	}
	renderX = (int)m_tInfo.fX - imageSize.x / 2 + (int)g_fScrollX;
	renderY = (int)m_tInfo.fY - imageSize.y / 2 + (int)g_fScrollY;
	UpdateRect();
	return 0;
}

void CP_Item::Render(HDC hDC)
{
	renderX = (int)m_tInfo.fX - imageSize.x / 2 + (int)g_fScrollX;
	renderY = (int)m_tInfo.fY - imageSize.y / 2 + (int)g_fScrollY;
	GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CP_Item::Release()
{
}

void CP_Item::SetColPlayer(bool flag)
{
	colPlayer = flag;
}

void CP_Item::SetItemType(P_ITEM_LIST _itemType)
{
	itemType = _itemType;
}

void CP_Item::PlayerUpdate()
{
	switch (itemType)
	{
	case NON:
		break;
	case SPOONBENDER:
		break;
	case CRICKETSHEAD:
		dynamic_cast<CPlayer*>(m_pTarget)->SetBulletColSize(60.f, 60.f);
		dynamic_cast<CPlayer*>(m_pTarget)->SetBulletImageSize(POINT{ (int)(62 * 1.5),  (int)(62 * 1.5) });
		dynamic_cast<CPlayer*>(m_pTarget)->SetHeadFlag(true);
		dynamic_cast<CPlayer*>(m_pTarget)->SetAtk(0.5f);
		break;
	case HALOOFFIES:
		CObjectMgr::GetInstance()->AddObject(SHIELD, CObjectFactory<CShield>::CreateObject(m_pTarget->GetInfo().fX - 50, m_pTarget->GetInfo().fY, 180.f));
		CObjectMgr::GetInstance()->AddObject(SHIELD, CObjectFactory<CShield>::CreateObject(m_pTarget->GetInfo().fX + 50, m_pTarget->GetInfo().fY, 360.f));
		break;
	case THEPACT:
		dynamic_cast<CPlayer*>(m_pTarget)->SetBulletImage(L"PACT_TEAR");
		dynamic_cast<CPlayer*>(m_pTarget)->SetAtk(0.5f);
		dynamic_cast<CPlayer*>(m_pTarget)->SetShotSpeed(0.7f);
		dynamic_cast<CPlayer*>(m_pTarget)->SetPact(true);
		break;
	case THEMULIGAN:
		break;
	case FATE:
		dynamic_cast<CPlayer*>(m_pTarget)->SetFateFlag(true);
		break;
	case LOSTCONTACT:
		break;
	case TWOTWO:
		dynamic_cast<CPlayer*>(m_pTarget)->SetDoubleFlag(true);
		break;
	}
}

void CP_Item::UpdateRect()
{
	m_tRect.left = m_tInfo.fX - m_tInfo.fCX *0.5f;
	m_tRect.top = m_tInfo.fY - m_tInfo.fCY *0.5f +50;
	m_tRect.right = m_tInfo.fX + m_tInfo.fCX *0.5f;
	m_tRect.bottom = m_tInfo.fY + m_tInfo.fCY *0.5f + 50;

}
