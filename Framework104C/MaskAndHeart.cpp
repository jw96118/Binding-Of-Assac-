#include "stdafx.h"
#include "MaskAndHeart.h"
#include "MonsterBullet.h"

CMaskAndHeart::CMaskAndHeart()
{

}


CMaskAndHeart::~CMaskAndHeart()
{
}

void CMaskAndHeart::Initialize()
{
	CImageMgr::GetInstance()->AddImage(L"MASKHEART", L"Image/monster/monster_187_maskandheart.png", 128, 64);
	CImageMgr::GetInstance()->AddImage(L"MASKHEART_2", L"Image/monster/monster_187_maskandheart_2.png", 128, 64);

	state = MONSTER_IDLE;
	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;

	imageSize.x = 80;
	imageSize.y = 80;

	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	shotSpeed = 30.f;
	range = 1000.f;

	m_fSpeed = 3.5f;
	type = MONSTER;
	hp = 10.f;

	srand((unsigned)time(NULL));
	deadX = 0;
	deadY = 0;
	deadCount = 0;
	monsterShieldP.x = m_tInfo.fX;
	monsterShieldP.y = m_tInfo.fY;
}

int CMaskAndHeart::Update()
{
	CMonster::UpdateShieldRect();
	if (m_bIsDead)
		return DEAD_OBJ;
	m_pTarget = CObjectMgr::GetInstance()->GetPlayer();

	if (deadCount > 10)
	{
		return DEAD_OBJ;
	}
	CGameObject::UpdateRect();
	if (hp <= 0)
	{
		m_tInfo.fCX = 0.f;
		m_tInfo.fCY = 0.f;

		state = MONSTER_DEAD;
	}
	if (state != MONSTER_DEAD)
		MonsterUpdate();
	CObjectMgr::GetInstance()->AddLayer(LAYER_OBJECT, this);

	return 0;
}

void CMaskAndHeart::Render(HDC hDC)
{
	CMonster::UpdateShieldRect();

	renderMaskX = monsterShieldP.x - 80 / 2 + (int)g_fScrollX;
	renderMaskY = monsterShieldP.y - 80 / 2 + (int)g_fScrollY;
	renderX = (int)m_tInfo.fX - imageSize.x / 2 + (int)g_fScrollX;
	renderY = (int)m_tInfo.fY - imageSize.y / 2 + (int)g_fScrollY;
	Animation(hDC);
}

void CMaskAndHeart::Release()
{
}

void CMaskAndHeart::Animation(HDC hDC)
{

	if (state < MONSTER_ATTACK_LEFT)
	{
		if (colBullet)
		{
			colBullet = false;
			hMemDC = CImageMgr::GetInstance()->GetDc(L"MASKHEART_2");
		}
		else
			hMemDC = CImageMgr::GetInstance()->GetDc(L"MASKHEART");
		switch (state)
		{
		case MONSTER_IDLE:
			break;
		case MONSTER_MOVE_LEFT:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, SIZEX * 2 + 6, 9, SIZEX, SIZEY + 1, RGB(0, 0, 0));
			GdiTransparentBlt(hDC, renderMaskX, renderMaskY, 80, 80, hMemDC, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_MOVE_RIGHT:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, SIZEX * 2 + 6, 9, SIZEX, SIZEY + 1, RGB(0, 0, 0));

			GdiTransparentBlt(hDC, renderMaskX, renderMaskY, 80, 80,
				hMemDC, 0, SIZEY, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_MOVE_TOP:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, SIZEX * 2 + 6, 9, SIZEX, SIZEY + 1, RGB(0, 0, 0));

			GdiTransparentBlt(hDC, renderMaskX, renderMaskY, 80, 80,
				hMemDC, SIZEX, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_MOVE_BOTTOM:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, SIZEX * 2 + 6, 9, SIZEX, SIZEY + 1, RGB(0, 0, 0));

			GdiTransparentBlt(hDC, renderMaskX, renderMaskY, 80, 80,
				hMemDC, SIZEX, SIZEY, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_ATTACK_LEFT:
			break;
		case MONSTER_ATTACK_RIGHT:
			break;
		case MONSTER_ATTACK_TOP:
			break;
		case MONSTER_ATTACK_BOTTOM:
			break;
		case MONSTER_DEAD:

			break;
		default:
			break;
		}
	}
	else if (state >= MONSTER_ATTACK_LEFT && state != MONSTER_DEAD)
	{
		switch (state)
		{
		case MONSTER_IDLE:
			break;
		case MONSTER_ATTACK_LEFT:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_ATTACK_RIGHT:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, SIZEX, SIZEY * 3, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_ATTACK_TOP:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, SIZEX * 2, SIZEY * 3, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case MONSTER_ATTACK_BOTTOM:
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, 0, SIZEY * 3, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		}
	}
	else if (state == MONSTER_DEAD)
	{
		hMemDC = CImageMgr::GetInstance()->GetDc(L"MONSTER_DEAD");
		CGameObject::PlayerSound(L"meaty deaths 3.wav", SOUNDMGR::MONSTER_DEAD);

		m_tInfo.fCX = 0;
		m_tInfo.fCY = 0;
		CGameObject::UpdateRect();
		GdiTransparentBlt(hDC, renderX, renderY, 100, 100, hMemDC, DEADIMAGEX*deadX, DEADIMAGEY*deadY, DEADIMAGEX, DEADIMAGEY, RGB(0, 0, 0));
		GdiTransparentBlt(hDC, renderMaskX, renderMaskY, 100, 100, hMemDC, DEADIMAGEX*deadX, DEADIMAGEY*deadY, DEADIMAGEX, DEADIMAGEY, RGB(0, 0, 0));
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 100)
		{
			deadCount++;
			deadX++;
			if (deadX > 4)
			{
				deadX = 0;
				deadY++;
			}
			dwOldTime_Render = dwCurTime_Render;
		}
	}
}

void CMaskAndHeart::MonsterUpdate()
{

	float makeShotPosX = m_tInfo.fX;
	float makeShotPosY = m_tInfo.fY;
	float w = m_pTarget->GetInfo().fX - m_tInfo.fX;
	float h = m_pTarget->GetInfo().fY - m_tInfo.fY;
	float d = sqrtf(w * w + h * h);
	if (d <= 300)
	{
		attackflag = true;

	}
	if (attackflag)
	{
		if (count <= 30)
		{
			imageSize.x -= 1;
			imageSize.y += 1;
		}
		else
		{
			imageSize.x += 1;
			imageSize.y -= 1;
		}
		if (count == 60)
		{
			CObjectMgr::GetInstance()->AddMonsterBullet(MONSTER_BULLET, makeShotPosX, makeShotPosY, 1, 0, range);
			CObjectMgr::GetInstance()->AddMonsterBullet(MONSTER_BULLET, makeShotPosX, makeShotPosY, -1, 0, range);
			CObjectMgr::GetInstance()->AddMonsterBullet(MONSTER_BULLET, makeShotPosX, makeShotPosY, 0, -1, range);
			CObjectMgr::GetInstance()->AddMonsterBullet(MONSTER_BULLET, makeShotPosX, makeShotPosY, 0, 1, range);
			count = 0;
			attackflag = false;
			CSoundMgr::GetInstance()->PlaySound(L"meat jumps 2.wav", SOUNDMGR::MONSTER);
		}
		count++;
	}
	if (state < MONSTER_ATTACK_LEFT)
	{
		m_fSpeed = 3.5f;
		if ((m_pTarget->GetInfo().fX <= m_tRect.right && m_pTarget->GetInfo().fX >= m_tRect.left)
			|| (m_pTarget->GetInfo().fY <= m_tRect.bottom && m_pTarget->GetInfo().fY >= m_tRect.top))
		{
			m_fSpeed = 6.f;
			if ((m_pTarget->GetInfo().fX <= m_tRect.right && m_pTarget->GetInfo().fX >= m_tRect.left))
			{
				if (m_pTarget->GetInfo().fY > m_tInfo.fY)
				{
					//state = MONSTER_ATTACK_BOTTOM;
				}
				else
				{
					//state = MONSTER_ATTACK_TOP;
				}
			}
			else if ((m_pTarget->GetInfo().fY <= m_tRect.bottom && m_pTarget->GetInfo().fY >= m_tRect.top))
			{
				if (m_pTarget->GetInfo().fX > m_tInfo.fX)
				{
					//state = MONSTER_ATTACK_RIGHT;
				}
				else
				{
					//state = MONSTER_ATTACK_LEFT;
				}
			}

		}
		dwCurTime_Update = GetTickCount();
		if (dwCurTime_Update - dwOldTime_Update >= 250)
		{
			state = (MONSTER_STATE)(rand() % 4 + 1);
			dwOldTime_Update = dwCurTime_Update;
		}
		switch (state)
		{
		case MONSTER_IDLE:
			break;
		case MONSTER_MOVE_LEFT:
			monsterShieldP.x -= m_fSpeed;
			break;
		case MONSTER_MOVE_RIGHT:
			monsterShieldP.x += m_fSpeed;
			break;
		case MONSTER_MOVE_TOP:
			monsterShieldP.y -= m_fSpeed;
			break;
		case MONSTER_MOVE_BOTTOM:
			monsterShieldP.y += m_fSpeed;
			break;
		case MONSTER_DEAD:
			break;
		default:
			break;
		}
	}
}
