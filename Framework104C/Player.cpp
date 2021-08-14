#include "stdafx.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "GuideBullet.h"
#include "Shield.h"
#include "Room.h"
#include "P_Item.h"
#include "Bomb.h"
#define FATEX 64
#define FATEY 52
bool shadow;
CPlayer::CPlayer()
	: m_bIsJump(false), m_fJumpForce(0.f), m_fJumpAcc(0.f), Now_Room_Info(nullptr)
{
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	imageSize.x = 80;
	imageSize.y = 80;
	fateImage.x = 140;
	fateImage.y = 140;
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;


	m_tInfo.fX = WINCX / 2.f - m_tInfo.fCX / 2.f;
	m_tInfo.fY = WINCY / 2.f - m_tInfo.fCY / 2.f;
	m_fSpeed = 6.0f;
	m_fAngle = 90.f;	// Degree	

	m_fJumpForce = 20.f;
	preStatus.shotSpeed = 30.f;
	preStatus.range = 500.f;
	CImageMgr::GetInstance()->AddImage(L"ISSAC", L"Image/character/character_001_isaac.png", 512, 512);
	CImageMgr::GetInstance()->AddImage(L"ISSAC_HURT", L"Image/character/character_001_isaac_red.png", 512, 512);
	CImageMgr::GetInstance()->AddImage(L"ISSAC_BLANK", L"Image/character/0.png", 0, 0);

	CImageMgr::GetInstance()->AddImage(L"MONSTER_DEAD", L"Image/effect/effect_002_bloodpoof.png", 256, 256);
	CImageMgr::GetInstance()->AddImage(L"EXPLOSION", L"Image/effect/effect_029_explosion.png", 384, 384);
	CImageMgr::GetInstance()->AddImage(L"FATE", L"Image/character/costume_179_fate.png", 480, 156);
	CImageMgr::GetInstance()->AddImage(L"FATE_BLACK", L"Image/character/costume_179_fate_black.png", 480, 156);
	CImageMgr::GetInstance()->AddImage(L"MAXHEAD", L"Image/character/costume_037_maxshead.png", 256, 32);
	CImageMgr::GetInstance()->AddImage(L"PACT", L"Image/character/costume_052_thepact_1.png", 256, 32);
	CImageMgr::GetInstance()->AddImage(L"TWOTWO", L"Image/character/costume_245_2020.png", 256, 32);

	CImageMgr::GetInstance()->AddImage(L"SHADOW", L"Image/effect/shadow_1.png", 120, 49);
	//CImageMgr::GetInstance()->GetImage(L"ISSAC");

	state = IDLE;
	atkState = NO;
	bodyAnimationCount = 0;

	aniWidth = 0;
	aniHeight = 0;

	dwOldTime_Render = GetTickCount();
	dwCurTime_Render = 0;

	dwOldTime_Update = GetTickCount();
	dwCurTime_Update = 0;

	dwOldTime_invin = GetTickCount();
	dwCurTime_invin = 0;
	invinCount = 0;

	type = PLAYER;
	scroll_Dir = NOT;
	hurtAniflag = false;

	moveTest = { 0,0 };

	hMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
	HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
	haveItem = false;

	bulletImageSize = { 80,80 };
	bulletColX = 40.f;
	bulletColY = 40.f;
	bulletImage = L"TEAR1";
	doubleflag = false;
	itemUpdate = false;
	pactflag = false;
	preStatus.maxHp = 6.f;
	preStatus.hp = preStatus.maxHp;
	preStatus.moveSpeed = m_fSpeed;
	preStatus.atk = 3.f;
	layer = LAYER_OBJECT;
	boomSize = 5;

	testSound = 1.f;
}

int CPlayer::Update()
{
	UpdateStatus();
	if (moveState == NO_MOVE)
	{
		if (!CSceneMgr::GetInstance()->GetCutScene())
		{
			KeyInput();
		}
		moveTest = { (int)m_tInfo.fX ,(int)m_tInfo.fY };
		scrollCount = 0;
	}
	else
	{
		float moveX = 0;
		float moveY = 0;
		if (scrollCount == 0)
		{
			switch (scroll_Dir)
			{
			case NOT:
				break;
			case LEFT:
				moveX = -150 - g_fScrollX;
				moveY = WINCY / 2 - g_fScrollY;
				break;
			case RIGHT:
				moveX = WINCX + 150 - g_fScrollX;
				moveY = WINCY / 2 - g_fScrollY;
				break;
			case TOP:
				moveX = WINCX / 2 - g_fScrollX;
				moveY = -150 - g_fScrollY;
				break;
			case BOTTOM:
				moveX = WINCX / 2 - g_fScrollX;
				moveY = WINCY + 150 - g_fScrollY;
				break;
			}
		}
		else
		{
			moveX = m_tInfo.fX;
			moveY = m_tInfo.fY;
		}
		m_tInfo.fX = moveX;
		m_tInfo.fY = moveY;
		scrollCount++;
	}
	CGameObject::UpdateRect();

	if (haveItem)
	{
		if (atkState != NO)
		{
			CGameObject::StopBGM(SOUNDMGR::PLAYER);
			atkState = NO;
		}
		CGameObject::PlayerSound(L"power up1.wav", SOUNDMGR::PLAYER);
		dwCurTime_Update = GetTickCount();
		if (dwCurTime_Update - dwOldTime_Update >= 2000)
		{
			haveItem = false;
			itemUpdate = true;
			dwOldTime_Update = dwCurTime_Update;
			CGameObject::StopBGM(SOUNDMGR::PLAYER);
		}
	}
	else
	{
		dwOldTime_Update = GetTickCount();
	}
	if (itemUpdate)
	{
		p_ItemList.back()->PlayerUpdate();
		itemUpdate = false;
	}

	CObjectMgr::GetInstance()->AddLayer(layer, this);
	Now_Room_Info->SetRenderMiniMpa(true);
	for (int i = 0; i < 4; i++)
	{
		if (Now_Room_Info->GetPeripheryRoom(i) != nullptr)
		{
			Now_Room_Info->GetPeripheryRoom(i)->SetRenderMiniMpa(true);
		}
	}

	if (headflag)
	{
		nowStatus.atk = preStatus.atk * 1.5f;
	}
	if (fateflag)
	{
		shadowY = 36;
	}
	else
	{
		shadowY = 18;
	}
	return NO_EVENT;
}
void CPlayer::Render(HDC hDC)
{
	CGameObject::UpdateRect();
	renderX = (int)m_tInfo.fX - imageSize.x / 2 + (int)g_fScrollX;
	renderY = (int)m_tInfo.fY - imageSize.y / 2 + (int)g_fScrollY;
	int SrenderX = (int)m_tInfo.fX - SHADOWX / 4 + (int)g_fScrollX;
	int SrenderY = (int)m_tInfo.fY - SHADOWY / 4 + (int)g_fScrollY;


	BLENDFUNCTION bf;
	bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
	bf.BlendFlags = 0; // 무조건 0이어야 한다
	bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미
	bf.SourceConstantAlpha = 127; // 투명도(투명 0 - 불투명 255)
	if (!shadow)
	{
		HDC tempDc2 = CreateCompatibleDC(NULL);
		HBITMAP paper2 = CreateCompatibleBitmap(hDC, WINCX, WINCY);
		HBITMAP oldPaper2 = (HBITMAP)SelectObject(tempDc2, paper2);
		DeleteObject(paper2);

		hMemDC = CImageMgr::GetInstance()->GetDc(L"ROOM_RENDER");

		GdiTransparentBlt(tempDc2, 0, 0, WINCX, WINCY,
			hMemDC, 0, 0, WINCX, WINCY, RGB(255, 255, 255));

		hMemDC = CImageMgr::GetInstance()->GetDc(L"SHADOW");

		GdiTransparentBlt(tempDc2, SrenderX, SrenderY + shadowY, SHADOWX / 2, SHADOWY / 2,
			hMemDC, 0, 0, SHADOWX, SHADOWY, RGB(255, 255, 255));

		GdiAlphaBlend(hDC, SrenderX, SrenderY + shadowY, SHADOWX / 2, SHADOWY / 2,
			tempDc2, SrenderX, SrenderY + shadowY, SHADOWX / 2, SHADOWY / 2, bf);

		DeleteDC(tempDc2);
	}
	UpdateInvin();
	Animation(hDC);

}
void CPlayer::Release()
{
}

void CPlayer::KeyInput()
{
	if (CKeyMgr::GetInstance()->KeyDown(KEY_R))
	{
		shadow = !shadow;
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_O))
	{
		testSound -= 0.1f;
		CSoundMgr::GetInstance()->SetVolume(SOUNDMGR::BGM, testSound);
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_P))
	{
		testSound += 0.1f;
		CSoundMgr::GetInstance()->SetVolume(SOUNDMGR::BGM, testSound);
	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_A) || CKeyMgr::GetInstance()->KeyPressing(KEY_S) || CKeyMgr::GetInstance()->KeyPressing(KEY_D) || CKeyMgr::GetInstance()->KeyPressing(KEY_W))
	{
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_A))
		{
			m_tInfo.fX -= m_fSpeed;
			state = MOVE_LEFT;
		}
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_D))
		{
			m_tInfo.fX += m_fSpeed;
			state = MOVE_RIGHT;
		}
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_W))
		{
			m_tInfo.fY -= m_fSpeed;
			state = MOVE_UP;

		}
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_S))
		{
			m_tInfo.fY += m_fSpeed;
			state = MOVE_DOWN;

		}
	}
	else
	{
		state = IDLE;
	}
	float makeShotPosX = m_tInfo.fX;
	float makeShotPosY = m_tInfo.fY - m_tInfo.fCY + 10;
	if (!haveItem)
	{
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_RIGHT))
		{
			if (bulletCount % (int)nowStatus.shotSpeed == 0)
			{
				CGameObject::PlayerSound(L"tear fire 4.wav", SOUNDMGR::PLAYER);
				if (doubleflag)
				{
					makeShotPosY = makeShotPosY - 10;
					CObjectMgr::GetInstance()->AddPlayerBullet(PLAYER_BULLET, makeShotPosX, makeShotPosY, 1, 0, nowStatus.range, bulletImageSize, bulletColX, bulletColY, bulletImage);
					makeShotPosY = makeShotPosY + 20;
				}
				CObjectMgr::GetInstance()->AddPlayerBullet(PLAYER_BULLET, makeShotPosX, makeShotPosY, 1, 0, nowStatus.range, bulletImageSize, bulletColX, bulletColY, bulletImage);
			}
			bulletCount++;
			atkState = ATTACK_RIGHT;
		}
		else if (CKeyMgr::GetInstance()->KeyPressing(KEY_LEFT))
		{
			if (bulletCount % (int)nowStatus.shotSpeed == 0)
			{
				CGameObject::PlayerSound(L"tear fire 4.wav", SOUNDMGR::PLAYER);
				if (doubleflag)
				{
					makeShotPosY = makeShotPosY - 10;
					CObjectMgr::GetInstance()->AddPlayerBullet(PLAYER_BULLET, makeShotPosX, makeShotPosY, -1, 0, nowStatus.range, bulletImageSize, bulletColX, bulletColY, bulletImage);
					makeShotPosY = makeShotPosY + 20;
				}
				CObjectMgr::GetInstance()->AddPlayerBullet(PLAYER_BULLET, makeShotPosX, makeShotPosY, -1, 0, nowStatus.range, bulletImageSize, bulletColX, bulletColY, bulletImage);
			}
			bulletCount++;
			atkState = ATTACK_LEFT;

		}
		else if (CKeyMgr::GetInstance()->KeyPressing(KEY_UP))
		{
			if (bulletCount % (int)nowStatus.shotSpeed == 0)
			{
				CGameObject::PlayerSound(L"tear fire 4.wav", SOUNDMGR::PLAYER);

				if (doubleflag)
				{
					makeShotPosX = m_tInfo.fX - 20;
					CObjectMgr::GetInstance()->AddPlayerBullet(PLAYER_BULLET, makeShotPosX, makeShotPosY, 0, -1, nowStatus.range, bulletImageSize, bulletColX, bulletColY, bulletImage);
					makeShotPosX = m_tInfo.fX + 20;
				}
				CObjectMgr::GetInstance()->AddPlayerBullet(PLAYER_BULLET, makeShotPosX, makeShotPosY, 0, -1, nowStatus.range, bulletImageSize, bulletColX, bulletColY, bulletImage);
			}
			bulletCount++;
			atkState = ATTACK_UP;
		}
		else if (CKeyMgr::GetInstance()->KeyPressing(KEY_DOWN))
		{
			if (bulletCount % (int)nowStatus.shotSpeed == 0)
			{
				CGameObject::PlayerSound(L"tear fire 4.wav", SOUNDMGR::PLAYER);
				if (doubleflag)
				{
					makeShotPosX = m_tInfo.fX - 20;
					CObjectMgr::GetInstance()->AddPlayerBullet(PLAYER_BULLET, makeShotPosX, makeShotPosY, 0, 1, nowStatus.range, bulletImageSize, bulletColX, bulletColY, bulletImage);
					makeShotPosX = m_tInfo.fX + 20;
				}
				CObjectMgr::GetInstance()->AddPlayerBullet(PLAYER_BULLET, makeShotPosX, makeShotPosY, 0, 1, nowStatus.range, bulletImageSize, bulletColX, bulletColY, bulletImage);
			}
			bulletCount++;
			atkState = ATTACK_DOWN;
		}
		else
		{
			bulletCount = 0;
			atkState = NO;
			CGameObject::StopBGM(SOUNDMGR::PLAYER);
		}
		if (CKeyMgr::GetInstance()->KeyDown(KEY_E))
		{
			if (boomSize > 0)
			{
				CObjectMgr::GetInstance()->AddObject(BOMB, CObjectFactory<CBomb>::CreateObject(makeShotPosX, makeShotPosY + 40));
				boomSize--;
			}
		}

	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_LBUTTON))
	{
		cout << Now_Room_Info->GetRoom_Type() << endl;

	}
}

void CPlayer::Animation(HDC hDC)
{
	//Graphics g(hDC);
	HDC tempDc = CreateCompatibleDC(NULL);
	HBITMAP paper = CreateCompatibleBitmap(hDC, WINCX, WINCY);
	HBITMAP oldPaper = (HBITMAP)SelectObject(tempDc, paper);
	DeleteObject(paper);
	faterenderX = (int)m_tInfo.fX - fateImage.x / 2 + (int)g_fScrollX;
	faterenderY = (int)m_tInfo.fY - fateImage.y / 2 + (int)g_fScrollY;

	if (!hurtAniflag && !haveItem)
	{
		//몸 애니메이션
		if (!fateflag)
		{
			if (invinState != INVINCIBILITY)
				hMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
			if (state != IDLE)
			{
				if (bodyAnimationCount >= 2)
				{
					if (state == MOVE_UP || state == MOVE_DOWN)
					{
						aniWidth = -2;
						aniHeight = SIZEY;
					}
					else
					{
						if (bodyAnimationCount >= 8)
						{
							aniWidth = -8;
							aniHeight = SIZEY * 3;
						}
					}
				}
				else if (bodyAnimationCount < 2)
				{
					if (state == MOVE_UP || state == MOVE_DOWN)
					{
						aniWidth = 6;
						aniHeight = 0;
					}
					else if (state == MOVE_LEFT || state == MOVE_RIGHT)
					{
						aniWidth = 0;
						aniHeight = SIZEY * 2;
					}
				}
				switch (state)
				{
				case MOVE_LEFT:
					//g.DrawImage(image, Rect(renderX + imageSize.x, renderY, -imageSize.x, imageSize.y), SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, UnitPixel);
					StretchBlt(tempDc, SIZEX, 0, -SIZEX, SIZEY, hMemDC, SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, SRCCOPY);
					GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, tempDc, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
					break;
				case MOVE_RIGHT:
					//	g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, UnitPixel);
					GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, RGB(0, 0, 0));
					break;
				case MOVE_UP:
				case MOVE_DOWN:
					//g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, UnitPixel);
					GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
						hMemDC, SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, RGB(0, 0, 0));
					break;
				}
			}
			dwCurTime_Render = GetTickCount();
			if (dwCurTime_Render - dwOldTime_Render >= 150)
			{
				bodyAnimationCount++;
				if (bodyAnimationCount > 9)
				{
					bodyAnimationCount = 0;
				}
				dwOldTime_Render = dwCurTime_Render;
			}
			switch (state)
			{
			case IDLE:
				//	g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), 0, SIZEY, SIZEX, SIZEY, UnitPixel);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
					hMemDC, 0, SIZEY, SIZEX, SIZEY, RGB(0, 0, 0));
				bodyAnimationCount = 0;
				aniWidth = 0;
				aniHeight = 0;
			}
		}
		else
		{
			if (invinState != INVINCIBILITY)
			{
				if (pactflag)
				{
					hMemDC = CImageMgr::GetInstance()->GetDc(L"FATE_BLACK");
				}
				else
				{
					hMemDC = CImageMgr::GetInstance()->GetDc(L"FATE");
				}
			}
			if (state == MOVE_DOWN || state == IDLE)
			{
				aniWidth = 0;
				aniHeight = 0;
			}
			else if (state == MOVE_UP)
			{
				aniWidth = 0;
				aniHeight = FATEY;
			}
			else if (state == MOVE_LEFT || state == MOVE_RIGHT)
			{
				aniWidth = 0;
				aniHeight = FATEY * 2;
			}

			switch (state)
			{
			case MOVE_LEFT:
				//g.DrawImage(image, Rect(renderX + imageSize.x, renderY, -imageSize.x, imageSize.y), SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, UnitPixel);
				StretchBlt(tempDc, FATEX, 0, -FATEX, FATEY, hMemDC, FATEX * (bodyAnimationCount), (int)aniHeight, FATEX, FATEY, SRCCOPY);
				GdiTransparentBlt(hDC, faterenderX, faterenderY, fateImage.x, fateImage.y, tempDc, 0, 0, FATEX, FATEY, RGB(0, 0, 0));
				break;
			case MOVE_RIGHT:
				GdiTransparentBlt(hDC, faterenderX, faterenderY, fateImage.x, fateImage.y, hMemDC, FATEX * (bodyAnimationCount), (int)aniHeight, FATEX, FATEY, RGB(0, 0, 0));
				break;
			case MOVE_UP:
				faterenderX = (int)m_tInfo.fX - imageSize.x / 2 + (int)g_fScrollX;
				faterenderY = (int)m_tInfo.fY - fateImage.y / 2 + (int)g_fScrollY;
				GdiTransparentBlt(hDC, faterenderX, faterenderY - 20, imageSize.x, fateImage.y,
					hMemDC, FATEX * 7, 0, 32, FATEY, RGB(0, 0, 0));
				faterenderX = (int)m_tInfo.fX - fateImage.x / 2 + (int)g_fScrollX;
				faterenderY = (int)m_tInfo.fY - fateImage.y / 2 + (int)g_fScrollY;

				GdiTransparentBlt(hDC, faterenderX, faterenderY, fateImage.x, fateImage.y,
					hMemDC, FATEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, FATEX, FATEY, RGB(0, 0, 0));
				break;
			case MOVE_DOWN:
			case IDLE:
				GdiTransparentBlt(hDC, faterenderX, faterenderY - 18, fateImage.x, fateImage.y,
					hMemDC, FATEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, FATEX, FATEY, RGB(0, 0, 0));
				break;
			}

			dwCurTime_Render = GetTickCount();
			if (dwCurTime_Render - dwOldTime_Render >= 150)
			{
				bodyAnimationCount++;
				if (bodyAnimationCount > 6)
				{
					bodyAnimationCount = 0;
				}
				dwOldTime_Render = dwCurTime_Render;
			}
			switch (state)
			{
				//case IDLE:
				//	//	g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), 0, SIZEY, SIZEX, SIZEY, UnitPixel);
				//	GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				//		hMemDC, 0, SIZEY, SIZEX, SIZEY, RGB(0, 0, 0));
				//	bodyAnimationCount = 0;
				//	aniWidth = 0;
				//	aniHeight = 0;
			}
		}
		if (invinState != INVINCIBILITY)
		{
			if (pactflag)
			{
				HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"PACT");
			}
			else if (headflag)
			{
				//머리 애니메이션
				HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"MAXHEAD");
			}
			else
			{
				HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
			}
		}

		//머리 애니메이션 공격 체크
		if (atkState != NO)
		{
			if (bulletCount % (int)nowStatus.shotSpeed == 0 || bulletCount == 1)
			{
				headAnimationCount = 1;
			}
			else if (bulletCount % (int)nowStatus.shotSpeed >= nowStatus.shotSpeed / 5)
			{
				headAnimationCount = 0;
			}
		}


		switch (atkState)
		{
		case NO:
			switch (state)
			{
			case IDLE:
				GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
					HeadhMemDC, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			case MOVE_LEFT:
				StretchBlt(tempDc, SIZEX, 0, -SIZEX, SIZEY, HeadhMemDC, SIZEX * 2, 0, SIZEX, SIZEY, SRCCOPY);
				GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
					tempDc, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			case MOVE_RIGHT:
				GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
					HeadhMemDC, SIZEX * 2, 0, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			case MOVE_UP:
				GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
					HeadhMemDC, SIZEX * 5, 0, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			case MOVE_DOWN:
				GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
					HeadhMemDC, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			default:
				break;
			}
			break;
		case ATTACK_LEFT:
			StretchBlt(tempDc, SIZEX, 0, -SIZEX, SIZEY, HeadhMemDC, SIZEX * (2 + headAnimationCount), 0, SIZEX, SIZEY, SRCCOPY);
			GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
				tempDc, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case ATTACK_RIGHT:
			GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
				HeadhMemDC, SIZEX * (2 + headAnimationCount), 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case ATTACK_UP:
			GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
				HeadhMemDC, SIZEX * (4 + headAnimationCount), 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		case ATTACK_DOWN:
			GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
				HeadhMemDC, SIZEX * headAnimationCount, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		}
		if (doubleflag)
		{
			HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"TWOTWO");
			switch (atkState)
			{
			case NO:
				switch (state)
				{
				case IDLE:
					//g.DrawImage(image, Rect(renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y), 0, 0, SIZEX, SIZEY, UnitPixel);
					GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
						HeadhMemDC, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
					break;
				case MOVE_LEFT:
					//g.DrawImage(image, Rect(renderX + imageSize.x, renderY - (int)m_tInfo.fCY + 15, -imageSize.x, imageSize.y), SIZEX * 2, 0, SIZEX, SIZEY, UnitPixel);
					StretchBlt(tempDc, SIZEX, 0, -SIZEX, SIZEY, HeadhMemDC, SIZEX * 2, 0, SIZEX, SIZEY, SRCCOPY);
					GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
						tempDc, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
					break;
				case MOVE_RIGHT:
					//	g.DrawImage(image, Rect(renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y), SIZEX * 2, 0, SIZEX, SIZEY, UnitPixel);
					GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
						HeadhMemDC, SIZEX * 2, 0, SIZEX, SIZEY, RGB(0, 0, 0));
					break;
				case MOVE_UP:
					//	g.DrawImage(image, Rect(renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y), SIZEX * 5, 0, SIZEX, SIZEY, UnitPixel); 
					GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
						HeadhMemDC, SIZEX * 5, 0, SIZEX, SIZEY, RGB(0, 0, 0));
					break;
				case MOVE_DOWN:
					//g.DrawImage(image, Rect(renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y), 0, 0, SIZEX, SIZEY, UnitPixel);
					GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
						HeadhMemDC, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
					break;
				default:
					break;
				}
				break;
			case ATTACK_LEFT:
				StretchBlt(tempDc, SIZEX, 0, -SIZEX, SIZEY, HeadhMemDC, SIZEX * (2 + headAnimationCount), 0, SIZEX, SIZEY, SRCCOPY);
				GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
					tempDc, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			case ATTACK_RIGHT:
				GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
					HeadhMemDC, SIZEX * (2 + headAnimationCount), 0, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			case ATTACK_UP:
				GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
					HeadhMemDC, SIZEX * (4 + headAnimationCount), 0, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			case ATTACK_DOWN:
				GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
					HeadhMemDC, SIZEX * headAnimationCount, 0, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			}
		}
	}
	else if (hurtAniflag)
	{
		if (!hurtSound)
		{
			CSoundMgr::GetInstance()->PlaySound(L"hurt grunt .wav", SOUNDMGR::EFFECT);
			hurtSound = true;
		}
		GdiTransparentBlt(hDC, renderX, renderY - 75, 150, 150,
			hMemDC, 140, 200, 60, 60, RGB(0, 0, 0));
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 300)
		{
			hurtAniflag = false;
			hurtSound = false;
			dwCurTime_Render = 0;
			dwOldTime_Render = GetTickCount();
			hMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
		}
	}
	else if (haveItem)
	{
		//머리 애니메이션
		hMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
		switch (state)
		{
		case IDLE:
		case MOVE_LEFT:
		case MOVE_RIGHT:
		case MOVE_UP:
		case MOVE_DOWN:
			GdiTransparentBlt(hDC, renderX, renderY - (int)m_tInfo.fCY + 15, imageSize.x, imageSize.y,
				hMemDC, SIZEX * 8, 0, SIZEX, SIZEY, RGB(0, 0, 0));
			break;
		}
		if (state != IDLE)
		{
			if (bodyAnimationCount >= 2)
			{
				if (state == MOVE_UP || state == MOVE_DOWN)
				{
					aniWidth = 6;
					aniHeight = SIZEY;
				}
				else
				{
					if (bodyAnimationCount >= 8)
					{
						aniWidth = 0;
						aniHeight = SIZEY * 3;
					}
				}
			}
			else if (bodyAnimationCount < 2)
			{
				if (state == MOVE_UP || state == MOVE_DOWN)
				{
					aniWidth = 14;
					aniHeight = 0;
				}
				else if (state == MOVE_LEFT || state == MOVE_RIGHT)
				{
					aniWidth = 8;
					aniHeight = SIZEY * 2;
				}
			}
			switch (state)
			{
			case MOVE_LEFT:
				//g.DrawImage(image, Rect(renderX + imageSize.x, renderY, -imageSize.x, imageSize.y), SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, UnitPixel);
				StretchBlt(tempDc, SIZEX, 0, -SIZEX, SIZEY, hMemDC, SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, SRCCOPY);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, tempDc, 0, 0, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			case MOVE_RIGHT:
				//	g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, UnitPixel);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y, hMemDC, SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			case MOVE_UP:
			case MOVE_DOWN:
				//g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, UnitPixel);
				GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
					hMemDC, SIZEX * (bodyAnimationCount + (int)aniWidth), (int)aniHeight, SIZEX, SIZEY, RGB(0, 0, 0));
				break;
			}
		}
		dwCurTime_Render = GetTickCount();
		if (dwCurTime_Render - dwOldTime_Render >= 150)
		{
			bodyAnimationCount++;
			if (bodyAnimationCount > 9)
			{
				bodyAnimationCount = 0;
			}
			dwOldTime_Render = dwCurTime_Render;
		}
		switch (state)
		{
		case IDLE:
			//	g.DrawImage(image, Rect(renderX, renderY, imageSize.x, imageSize.y), 0, SIZEY, SIZEX, SIZEY, UnitPixel);
			GdiTransparentBlt(hDC, renderX, renderY, imageSize.x, imageSize.y,
				hMemDC, SIZEX * 8, SIZEY, SIZEX, SIZEY, RGB(0, 0, 0));
			bodyAnimationCount = 0;
			aniWidth = 0;
			aniHeight = 0;
		}
	}
	DeleteDC(tempDc);
}

void CPlayer::UpdateInvin()
{
	if (invinState == INVINCIBILITY)
	{
		if (dwCurTime_invin == 0)
		{
			hurtAniflag = true;
			dwCurTime_Render = 0;
			dwOldTime_Render = GetTickCount();
			dwOldTime_invin = GetTickCount();
		}
		if (hurtAniflag)
		{
			if (invinCount % 2 == 0)
			{
				//image = CImageMgr::GetInstance()->GetImage(L"ISSAC_HURT");
				hMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC_HURT");

			}
			else
			{
				//image = CImageMgr::GetInstance()->GetImage(L"ISSAC");
				hMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
				if (pactflag)
				{
					HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"PACT");
				}
				else if (headflag)
				{
					//머리 애니메이션
					HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"MAXHEAD");
				}
				else
				{
					HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
				}
			}
		}
		else
		{
			if (invinCount % 2 == 0)
			{
				//image = CImageMgr::GetInstance()->GetImage(L"BLANK");
				hMemDC = CImageMgr::GetInstance()->GetDc(L"BLANK");
				HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"BLANK");
			}
			else
			{
				//image = CImageMgr::GetInstance()->GetImage(L"ISSAC");
				hMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
				if (fateflag)
				{
					if (pactflag)
					{
						hMemDC = CImageMgr::GetInstance()->GetDc(L"FATE_BLACK");
					}
					else
					{
						hMemDC = CImageMgr::GetInstance()->GetDc(L"FATE");
					}
				}
				if (pactflag)
				{
					HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"PACT");
				}
				else if (headflag)
				{
					//머리 애니메이션
					HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"MAXHEAD");
				}
				else
				{
					HeadhMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
				}
			}
		}
		invinCount++;

		dwCurTime_invin = GetTickCount();
		if (dwCurTime_invin - dwOldTime_invin >= 2000)
		{
			invinState = NOT_INVINCIBILITY;
			dwOldTime_invin = dwCurTime_invin;
			dwCurTime_invin = 0;
			invinCount = 0;
			hMemDC = CImageMgr::GetInstance()->GetDc(L"ISSAC");
		}
	}

}

void CPlayer::UpdateStatus()
{
	nowStatus.atk = preStatus.atk;
	nowStatus.hp = preStatus.hp;
	nowStatus.maxHp = preStatus.maxHp;
	nowStatus.range = preStatus.range;
	nowStatus.moveSpeed = preStatus.moveSpeed;
	nowStatus.bulletSpeed = preStatus.bulletSpeed;
	nowStatus.shotSpeed = preStatus.shotSpeed;

}

void CPlayer::SetMoveState(PLAYER_MOVE_STATE _moveState)
{
	moveState = _moveState;
}

PLAYER_MOVE_STATE CPlayer::GetMoveState()
{
	return moveState;
}

CRoom * CPlayer::GetNowRoomInfo()
{
	return Now_Room_Info;
}

PLAYER_INVIL_STATE CPlayer::GetInvilState()
{
	return invinState;
}

const bool & CPlayer::GetHaveItem() const
{
	return haveItem;
}

const bool & CPlayer::GetFate() const
{
	return fateflag;
}

const bool & CPlayer::GetItemUpdate() const
{
	return itemUpdate;
}

const float & CPlayer::GetMaxHp() const
{
	return nowStatus.maxHp;
}

const float & CPlayer::GetHp() const
{
	return nowStatus.hp;
}

const float & CPlayer::GetAtk() const
{
	return nowStatus.atk;
}

const int & CPlayer::GetBombSize() const
{
	return boomSize;
}

const STATUS & CPlayer::GetStatus() const
{
	return nowStatus;
}

void CPlayer::SetNowRoomInfo(CRoom * room)
{
	Now_Room_Info = room;
}

void CPlayer::SetScrollDir(ROOM_DIRECTION _scroll_Dir)
{
	scroll_Dir = _scroll_Dir;
}

void CPlayer::SetInvinState(PLAYER_INVIL_STATE _invinState)
{
	invinState = _invinState;
}

void CPlayer::SetHp(int _hp)
{
	preStatus.hp += _hp;
}

void CPlayer::SetP_Item(CP_Item* _pItem)
{
	p_ItemList.push_back(_pItem);
}

void CPlayer::SetHaveItem(bool flag)
{
	haveItem = flag;
}

void CPlayer::SetBulletImage(wstring _image)
{
	bulletImage = _image;
}

void CPlayer::SetBulletImageSize(POINT _pt)
{
	bulletImageSize = _pt;
}

void CPlayer::SetBulletColSize(float x, float y)
{
	bulletColX = x;
	bulletColY = y;
}

void CPlayer::SetDoubleFlag(bool flag)
{
	doubleflag = flag;
}

void CPlayer::SetFateFlag(bool flag)
{
	fateflag = flag;
}

void CPlayer::SetHeadFlag(bool flag)
{
	headflag = flag;
}

void CPlayer::SetAtk(float a)
{
	preStatus.atk += a;
}

void CPlayer::SetShotSpeed(float a)
{
	preStatus.shotSpeed += a;
}

void CPlayer::SetLayer(RENDER_LAYER _layer)
{
	layer = _layer;
}

void CPlayer::SetItemUpdate(bool flag)
{
	itemUpdate = flag;
}

void CPlayer::SetPact(bool flag)
{
	pactflag = flag;
}

