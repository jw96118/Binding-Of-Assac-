#include "stdafx.h"
#include "BossUI.h"
#include "Monster.h"

CBossUI::CBossUI()
{

}


CBossUI::~CBossUI()
{
}

void CBossUI::Initialize()
{
	CImageMgr::GetInstance()->AddImage(L"BOSS_UI", L"Image/ui/ui_bosshealthbar_3.png", 150, 64);
	CImageMgr::GetInstance()->AddImage(L"BOSS_HP", L"Image/ui/Boss_HP.png", 599, 102);
	imageSize.x = 150 * 3;
	imageSize.y = 32 * 3;
	m_tInfo.fX = 850;
	m_tInfo.fY = 120;
	boss_hp = 0;
}

int CBossUI::Update()
{
	m_pTarget = CObjectMgr::GetInstance()->GetBoss();
	CObjectMgr::GetInstance()->AddLayer(LAYER_UI, this);
	if (m_pTarget != nullptr)
	{
		boss_hp = 4.5f * (dynamic_cast<CMonster*>(m_pTarget)->GetMonsterHP() / 2.5f);
	}
	return 0;
}

void CBossUI::Render(HDC hDC)
{
	int renderX;
	int renderY;
	int x = 0;
	int y = 0;
	renderX = (int)m_tInfo.fX - (imageSize.x);
	renderY = (int)m_tInfo.fY - (imageSize.y);
	hMemDC = CImageMgr::GetInstance()->GetDc(L"BOSS_UI");
	GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
		hMemDC, 0, 32, 150, 32, RGB(255, 255, 255));
	
	hMemDC = CImageMgr::GetInstance()->GetDc(L"BOSS_HP");
	GdiTransparentBlt(hDC, renderX + 20, renderY, (int)boss_hp + 8 , imageSize.y,
		hMemDC, 8, 0, 150, 32, RGB(0, 0, 0));
	
	hMemDC = CImageMgr::GetInstance()->GetDc(L"BOSS_UI");
	GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
		hMemDC, 0, 0, 150, 32, RGB(255, 255, 255));
	
	
}

void CBossUI::Release()
{
}
