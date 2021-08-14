#include "stdafx.h"
#include "Monstro.h"
#include "MonstroBullet_1.h"

CMonstro::CMonstro()
{
}


CMonstro::~CMonstro()
{
}

void CMonstro::Initialize()
{
	state = MONSTER_IDLE;
	dwOldTime_Update = GetTickCount();
	dwOldTime_Render = GetTickCount();
	dwCurTime_Update = 0;
	dwCurTime_Render = 0;
	CImageMgr::GetInstance()->AddImage(L"MONSTRO", L"Image/boss/monstro/boss_004_monstro.png", 400, 224);
	CImageMgr::GetInstance()->AddImage(L"MONSTRO_2", L"Image/boss/monstro/boss_004_monstro_2.png", 400, 224);
	CImageMgr::GetInstance()->AddImage(L"MONSTRO_DEAD", L"Image/effect/effect_077.png", 1817, 101);
	imageSize.x = MONSTROIMAGEX * 2.5;
	imageSize.y = MONSTROIMAGEY *2.5;

	m_tInfo.fCX = MONSTROIMAGEX * 2 -50;

	shotSpeed = 30.f;
	range = 1000.f;

	m_fSpeed = 6.f;
	type = MONSTER;
	hp = 250.f;
	srand((unsigned)time(NULL));
	deadX = 0;
	deadY = 0;
	deadCount = 0;
	pattern = BATTLE_START;
	prePattern = pattern;
	aniCountX = 0;
	aniCountY = 0;

	state = MONSTER_MOVE_LEFT;

	nowShot = false;
	nowJump = false;
	bulletMake = false;
	longJump = false;
	deadCount = 0;
	deadBloodCount = 0;
	m_pTarget = CObjectMgr::GetInstance()->GetPlayer();
	sSizeX = SHADOWX / 4;
	sSizeY = SHADOWY / 4;
}

int CMonstro::Update()
{
	UpdateStartCheck = true;
	if (m_bIsDead)
		return DEAD_OBJ;
	m_pTarget = CObjectMgr::GetInstance()->GetPlayer();
	
	if (deadCount1 > 1000)
	{
		endingflag = true;
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
	m_tRect.top = m_tInfo.fY + 30;
	m_tRect.bottom = m_tInfo.fY + (MONSTROIMAGEY * 2.5) * 0.5;
	CObjectMgr::GetInstance()->AddLayer(LAYER_OBJECT, this);
	return NO_EVENT;
}

void CMonstro::Render(HDC hDC)
{

	CGameObject::UpdateRect();
	m_tRect.top = m_tInfo.fY + 30;
	m_tRect.bottom = m_tInfo.fY + (MONSTROIMAGEY * 2.5) * 0.5;
	renderX = (int)m_tInfo.fX - imageSize.x / 2 + (int)g_fScrollX;
	renderY = (int)m_tInfo.fY - imageSize.y / 2 + (int)g_fScrollY;

	int SrenderX = (int)m_tInfo.fX - sSizeX + (int)g_fScrollX;
	int SrenderY = (int)m_tInfo.fY - sSizeY + (int)g_fScrollY + shadowY;


	BLENDFUNCTION bf;
	bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
	bf.BlendFlags = 0; // 무조건 0이어야 한다
	bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미
	bf.SourceConstantAlpha = 127; // 투명도(투명 0 - 불투명 255)
	if (!shadow)
	{
		if (state != MONSTER_DEAD)
		{
			HDC tempDc2 = CreateCompatibleDC(NULL);
			HBITMAP paper2 = CreateCompatibleBitmap(hDC, WINCX, WINCY);
			HBITMAP oldPaper2 = (HBITMAP)SelectObject(tempDc2, paper2);
			DeleteObject(paper2);

			hMemDC = CImageMgr::GetInstance()->GetDc(L"ROOM_RENDER");

			GdiTransparentBlt(tempDc2, 0, 0, WINCX, WINCY,
				hMemDC, 0, 0, WINCX, WINCY, RGB(255, 255, 255));

			hMemDC = CImageMgr::GetInstance()->GetDc(L"SHADOW");

			GdiTransparentBlt(tempDc2, SrenderX - 60, SrenderY + 100, (int)((sSizeX * 4) * 1.5), (int)((sSizeY * 4)  * 1.5),
				hMemDC, 0, 0, SHADOWX, SHADOWY, RGB(255, 255, 255));

			GdiAlphaBlend(hDC, SrenderX - 60, SrenderY + 100, (int)((sSizeX * 4)  * 1.5), (int)((sSizeY * 4)  * 1.5),
				tempDc2, SrenderX - 60, SrenderY + 100, (int)((sSizeX * 4)  * 1.5), (int)((sSizeY * 4)  * 1.5), bf);

			DeleteDC(tempDc2);
		}
	}

	Animation(hDC);
}

void CMonstro::Release()
{
}

void CMonstro::Animation(HDC hDC)
{
	HDC tempDc = CreateCompatibleDC(NULL);
	HBITMAP paper = CreateCompatibleBitmap(hDC, WINCX, WINCY);
	HBITMAP oldPaper = (HBITMAP)SelectObject(tempDc, paper);
	DeleteObject(paper);
	if (state != MONSTER_DEAD)
	{
		if (colBullet)
		{
			colBullet = false;
			hMemDC = CImageMgr::GetInstance()->GetDc(L"MONSTRO_2");
		}
		else
		hMemDC = CImageMgr::GetInstance()->GetDc(L"MONSTRO");
		switch (pattern)
		{
		case BATTLE_START:
			if (state == MONSTER_MOVE_LEFT)
			{
				StretchBlt(tempDc, MONSTROIMAGEX, 0, -MONSTROIMAGEX, MONSTROIMAGEY, hMemDC, MONSTROIMAGEX * 2, 0, MONSTROIMAGEX, MONSTROIMAGEY, SRCCOPY);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, tempDc, 0, 0, MONSTROIMAGEX, MONSTROIMAGEY, RGB(0, 0, 0));
			}
			else
			{
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
					hMemDC, MONSTROIMAGEX * 2, 0, MONSTROIMAGEX, MONSTROIMAGEY, RGB(0, 0, 0));
			}
			dwCurTime_Update = GetTickCount();
			if (dwCurTime_Update - dwOldTime_Update >= 700)
			{
				if (m_pTarget->GetInfo().fX >= m_tInfo.fX)
				{
					state = MONSTER_MOVE_LEFT;
				}
				else if (m_pTarget->GetInfo().fX < m_tInfo.fX)
					state = MONSTER_MOVE_RIGHT;
				switch (prePattern)
				{
				case BATTLE_START:
					pattern = SHORT_JUMP;
					break;
				case SHORT_JUMP:
					pattern = LONG_JUMP;
					break;
				case LONG_JUMP:
					pattern = SHOT_TEAR;
					break;
				case SHOT_TEAR:
					pattern = SHORT_JUMP;
					break;
				default:
					break;
				}

				prePattern = pattern;

				aniCountX = 0;
				aniCountY = 1;
				preTarget = m_pTarget->GetInfo();
				dwOldTime_Update = GetTickCount();
				dwCurTime_Update = 0;
			}
			break;
		case SHORT_JUMP:
			if (state == MONSTER_MOVE_LEFT)
			{
				StretchBlt(tempDc, MONSTROIMAGEX, 0, -MONSTROIMAGEX, MONSTROIMAGEY, hMemDC, MONSTROIMAGEX * aniCountX, MONSTROIMAGEY * aniCountY, MONSTROIMAGEX, MONSTROIMAGEY, SRCCOPY);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, tempDc, 0, 0, MONSTROIMAGEX, MONSTROIMAGEY, RGB(0, 0, 0));
			}
			else
			{
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
					hMemDC, MONSTROIMAGEX * aniCountX, MONSTROIMAGEY * aniCountY, MONSTROIMAGEX, MONSTROIMAGEY, RGB(0, 0, 0));
			}
			if (nowJump)
			{
				
				dwCurTime_Render = GetTickCount();
				if (dwCurTime_Render - dwOldTime_Render >= 100)
				{
					aniCountX++;
					if (aniCountX >= 2)
					{
						aniCountX = 2;

					}
					dwOldTime_Render = dwCurTime_Render;
				}
				if (aniCountX >= 2)
				{
					m_tInfo.fCX = 0;
					m_tInfo.fY -= 20.f;
					shadowY += 20;
					jumpMove();
				}
			}
			else
			{
			
				jumpMove();

				dwCurTime_Render = GetTickCount();
				if (dwCurTime_Render - dwOldTime_Render >= 100)
				{
					if (aniCountX == 2)
					{
						m_tInfo.fY += 14.f;
						shadowY -= 14;
						aniCountX = 4;
						aniCountY = 0;
						CGameObject::StopBGM(SOUNDMGR::MONSTER);

					}
					else if (aniCountX == 4)
					{
						m_tInfo.fY += 20.f;
						shadowY -= 20;
						aniCountX = 0;
						aniCountY = 1;
						m_tInfo.fCX = MONSTROIMAGEX * 2 - 50;
						CGameObject::PlayerSound(L"meat jumps 2.wav", SOUNDMGR::MONSTER);

					}
					else if (aniCountX == 0)
					{
					
						aniCountX = 2;
						aniCountY = 0;
						shadowY = 0;
						pattern = BATTLE_START;
						dwOldTime_Update = GetTickCount();
						dwCurTime_Update = 0;
					}
					dwOldTime_Render = dwCurTime_Render;
				}
			}
			break;
		case LONG_JUMP:
			if (state == MONSTER_MOVE_LEFT)
			{
				StretchBlt(tempDc, MONSTROIMAGEX, 0, -MONSTROIMAGEX, MONSTROIMAGEY, hMemDC, MONSTROIMAGEX * aniCountX, MONSTROIMAGEY * aniCountY, MONSTROIMAGEX, MONSTROIMAGEY, SRCCOPY);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, tempDc, 0, 0, MONSTROIMAGEX, MONSTROIMAGEY, RGB(0, 0, 0));
			}
			else
			{
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
					hMemDC, MONSTROIMAGEX * aniCountX, MONSTROIMAGEY * aniCountY, MONSTROIMAGEX, MONSTROIMAGEY, RGB(0, 0, 0));
			}
			if (nowJump)
			{
				dwCurTime_Render = GetTickCount();
				if (dwCurTime_Render - dwOldTime_Render >= 10)
				{
					aniCountX++;
					if (aniCountX >= 1)
					{
						aniCountX = 1;
					}
				}
				if (aniCountX >= 1)
				{
					m_tInfo.fCX = 0;
					m_tInfo.fY -= 20.f;
					shadowY += 20;
					sSizeX -= 1;
					sSizeY -= 1;
					if (!longJump)
					{
						longJump = true;
						
						dwOldTime_Update = dwCurTime_Render;
					}
				}
			}
			else
			{

				if (m_tInfo.fY > preTarget.fY - 100)
				{
					m_tInfo.fY = preTarget.fY - 100;
					shadowY = 0;
					sSizeX = SHADOWX / 4;
					sSizeY = SHADOWY / 4;
					longJump = false;
				}
				dwCurTime_Render = GetTickCount();
				if (dwCurTime_Render - dwOldTime_Render >= 10)
				{
					if (longJump)
					{
						m_tInfo.fX = preTarget.fX;
						m_tInfo.fY += 20.f;
						shadowY -= 20;
						sSizeX += 2;
						sSizeY += 2;
						if (sSizeX >= SHADOWX / 4)
						{
							sSizeX = SHADOWX / 4;
						}
						if (sSizeY >= SHADOWX / 4)
						{
							sSizeY = SHADOWX / 4;
						}
						aniCountX = 4;
						aniCountY = 0;
						CGameObject::StopBGM(SOUNDMGR::MONSTER);
					
					}
					else if (aniCountX == 4 && !longJump)
					{
						CGameObject::PlayerSound(L"forest boss stomp 2.wav", SOUNDMGR::MONSTER);

						MakeBullet_1();
						aniCountX = 0;
						aniCountY = 1;
						m_tInfo.fCX = MONSTROIMAGEX * 2 - 50;

					}
					else if (aniCountX == 0)
					{
						aniCountX = 2;
						aniCountY = 0;
						pattern = BATTLE_START;
						dwOldTime_Update = GetTickCount();
						dwCurTime_Update = 0;
						
					}
					dwOldTime_Render = dwCurTime_Render;
				}
			}
			break;
		case SHOT_TEAR:
			if (state == MONSTER_MOVE_LEFT)
			{
				StretchBlt(tempDc, MONSTROIMAGEX, 0, -MONSTROIMAGEX, MONSTROIMAGEY, hMemDC, MONSTROIMAGEX * aniCountX, 0, MONSTROIMAGEX, MONSTROIMAGEY, SRCCOPY);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, tempDc, 0, 0, MONSTROIMAGEX, MONSTROIMAGEY, RGB(0, 0, 0));
			}
			else
			{
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
					hMemDC, MONSTROIMAGEX * aniCountX, 0, MONSTROIMAGEX, MONSTROIMAGEY, RGB(0, 0, 0));
			}
			if (nowShot)
			{
				dwCurTime_Render = GetTickCount();
				aniCountX = 1;
				if (dwCurTime_Render - dwOldTime_Render >= 400)
				{
					nowShot = false;
					dwOldTime_Render = dwCurTime_Render;
					CGameObject::StopBGM(SOUNDMGR::MONSTER);
				}

			}
			else
			{
				CGameObject::PlayerSound(L"boss lite sloppy roar 3.wav", SOUNDMGR::MONSTER);
				MakeBullet_2();
				aniCountX = 3;
				dwCurTime_Render = GetTickCount();
				if (dwCurTime_Render - dwOldTime_Render >= 500)
				{
					bulletMake = false;
					UpdateStartCheck = false;
					aniCountX = 2;
					aniCountY = 0;
					pattern = BATTLE_START;
					dwOldTime_Update = GetTickCount();
					dwCurTime_Update = 0;
					dwOldTime_Render = dwCurTime_Render;
				}
			}

			break;
		}
	}

    else
	{	
		if (deadCount1 <= 300)
		{
			if (deadCount1 == 0)
			{
				renderX -= 10;
			}
			else if (deadCount1 % 2 == 0)
			{
				renderX -= 15;
			}
			else
			{
				renderX += 15;
			}
			hMemDC = CImageMgr::GetInstance()->GetDc(L"MONSTRO");
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, MONSTROIMAGEX * 3, MONSTROIMAGEY, MONSTROIMAGEX, MONSTROIMAGEY, RGB(0, 0, 0));
			
			switch (bloodCount)
			{
			case 0:
				blood.x = (int)m_tInfo.fX - 100 + (int)g_fScrollX;
				blood.y = (int)m_tInfo.fY + 20 + (int)g_fScrollY;
				break;
			case 1:
				blood.x = (int)m_tInfo.fX + (int)g_fScrollX;
				blood.y = (int)m_tInfo.fY + 40 + (int)g_fScrollY;
				break;
			case 2:
				blood.x = (int)m_tInfo.fX - 100 + (int)g_fScrollX;
				blood.y = (int)m_tInfo.fY + 60 + (int)g_fScrollY;
				break;
			case 3:
				blood.x = (int)m_tInfo.fX + 60 + (int)g_fScrollX;
				blood.y = (int)m_tInfo.fY + 60 + (int)g_fScrollY;
				break;
			}
			hMemDC = CImageMgr::GetInstance()->GetDc(L"MONSTER_DEAD");
			CGameObject::PlayerSound(L"meaty deaths 0.wav", SOUNDMGR::MONSTER_DEAD);

			GdiTransparentBlt(hDC, blood.x, blood.y, 100, 100, hMemDC, DEADIMAGEX*deadX, DEADIMAGEY*deadY, DEADIMAGEX, DEADIMAGEY, RGB(0, 0, 0));
			dwCurTime_Render = GetTickCount();
			if (dwCurTime_Render - dwOldTime_Render >= 50)
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
			if (deadCount >= 10)
			{
				bloodCount++;
				CGameObject::StopBGM(SOUNDMGR::MONSTER_DEAD);

				deadCount = 0;
				deadY = 0;
				if (bloodCount > 3)
				{
					deadCount1 = 300;
				}
			}
		}
		else
		{
			hMemDC = CImageMgr::GetInstance()->GetDc(L"MONSTRO_DEAD");
			CGameObject::PlayerSound(L"meaty deaths 3.wav", SOUNDMGR::MONSTER_DEAD);

			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, 150 * deadBloodCount,0, 150, 100, RGB(0, 0, 0));
			dwCurTime_Render = GetTickCount();
			/*if(CKeyMgr::GetInstance()->KeyDown(KEY_RBUTTON))
			{
				deadBloodCount++;
			}*/
			if (dwCurTime_Render - dwOldTime_Render >= 100)
			{
				deadBloodCount++;
				dwOldTime_Render = dwCurTime_Render;
				if (deadBloodCount > 12)
				{
					deadCount1 = 1000;
				}
			}
		}
		deadCount1++;
	}
	DeleteDC(tempDc);
}

void CMonstro::MonsterUpdate()
{
	if (!nowMove)
	{
		switch (pattern)
		{
		case BATTLE_START:
			break;
		case SHORT_JUMP:
			if (dwCurTime_Update == 0)
			{
				nowJump = true;
				dwOldTime_Render = GetTickCount();
				dwCurTime_Render = 0;
			}
			if (nowJump)
			{
				aniCountY = 1;
				dwCurTime_Update = GetTickCount();
				if (dwCurTime_Update - dwOldTime_Update >= 330)
				{
					nowJump = false;
					dwOldTime_Update = dwCurTime_Update;
					dwOldTime_Render = GetTickCount();
					dwCurTime_Render = 0;
				}
			}
			break;
		case LONG_JUMP:
			if (dwCurTime_Update == 0)
			{
				nowJump = true;
				dwOldTime_Render = GetTickCount();
				dwCurTime_Render = 0;
				bulletMake = false;
			}
			if (nowJump)
			{
				aniCountY = 1;
				dwCurTime_Update = GetTickCount();
				if (dwCurTime_Update - dwOldTime_Update >= 930)
				{
					nowJump = false;
					shadowY = WINCY;
					preTarget = m_pTarget->GetInfo();
					m_tInfo.fY = preTarget.fY - WINCY -100;
					dwOldTime_Update = dwCurTime_Update;
					dwOldTime_Render = GetTickCount();
					dwCurTime_Render = 0;
				}
			}
			break;
		case SHOT_TEAR:
			if (dwCurTime_Update == 0)
			{
				nowShot = true;
				bulletMake = false;
				dwOldTime_Render = GetTickCount();
				dwCurTime_Render = 0;
			}
			if (nowShot)
			{
				dwCurTime_Update = GetTickCount();
				if (dwCurTime_Update - dwOldTime_Update >= 930)
				{
					dwOldTime_Update = dwCurTime_Update;
				}
			}
			break;
		}
	
	}
}

void CMonstro::jumpMove()
{
	w = preTarget.fX - m_tInfo.fX;
	h = preTarget.fY - m_tInfo.fY;
	d = sqrtf(w * w + h * h);

	m_fAngle = acosf(w / d);

	if (m_tInfo.fY < m_pTarget->GetInfo().fY)
		m_fAngle *= -1.f;
	m_tInfo.fX += cosf(m_fAngle) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle) * m_fSpeed;
	shadowY += sinf(m_fAngle) * m_fSpeed;
}

void CMonstro::MakeBullet_1()
{
	float makeShotPosX = m_tInfo.fX;
	float makeShotPosY = m_tInfo.fY + 70;
	int targetX;
	int targetY;
	if (!bulletMake)
	{
		for (int i = 0; i < 15; i++)
		{
			range = rand() % 200 + 300;
			if (i % 3 == 0)
			{
				targetX = (int)m_pTarget->GetInfo().fX + rand() % 100;
				targetY = (int)m_pTarget->GetInfo().fY + rand() % 100;
			}
			else if (i % 3 == 1)
			{
				targetX = 100 + rand() % 100;
				targetY = 100 + rand() % 100;
			}
			else if (i % 3 == 2)
			{
				targetX = 1000 + rand() % 100;
				targetY = 600 + rand() % 100;
			}
			CObjectMgr::GetInstance()->AddMonstroBullet(MONSTRO_BULLET, makeShotPosX, makeShotPosY, targetX, targetY, range);
		}
		bulletMake = true;
	}

}

void CMonstro::MakeBullet_2()
{
	float makeShotPosX = m_tInfo.fX;
	float makeShotPosY = m_tInfo.fY + 70;
	int targetX;
	int targetY;
	srand((unsigned)time(NULL));
	if (!bulletMake)
	{
		for (int i = 0; i < 10; i++)
		{
			range = rand() % 200+300;
			targetX = (int)m_pTarget->GetInfo().fX + rand() % 100;
			targetY = (int)m_pTarget->GetInfo().fY + rand() % 100;
			CObjectMgr::GetInstance()->AddMonstroBullet(MONSTRO_BULLET, makeShotPosX, makeShotPosY, targetX, targetY, range);
		}
		bulletMake = true;
	}

}
