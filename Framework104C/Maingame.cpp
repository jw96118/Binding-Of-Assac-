#include "stdafx.h"
#include "Maingame.h"
#include "Player.h"
#include "Monster.h"
#include "BmpMgr.h"
#include "UI.h"
#include "MiniMap.h"
#include "BossUI.h"


// юс╫ц
float g_fScrollX = 0.f;
float g_fScrollY = 0.f;
CMaingame::CMaingame()
{
}

CMaingame::~CMaingame()
{
	Release();
}

void CMaingame::Initialize()
{
	CSoundMgr::GetInstance()->Initialize();
	//gdi+
	Gdiplus::GdiplusStartupInput         m_GdiplusStartupInput;
	ULONG_PTR                   m_GdiplusToken;
	GdiplusStartup(&m_GdiplusToken, &m_GdiplusStartupInput, NULL);
	// Player Image
	//CBmpMgr::GetInstance()->LoadBmp(L"Player", L"../Framework104C/Image/bmpcharater/character_001_isaac.bmp");
	srand(unsigned(time(nullptr)));

	m_hDC = GetDC(g_hWnd);	

	// Player	
	CObjectMgr::GetInstance()->AddObject(PLAYER, CObjectFactory<CPlayer>::CreateObject()); 

	CObjectMgr::GetInstance()->AddObject(UI, CObjectFactory<CMiniMap>::CreateObject());
	CObjectMgr::GetInstance()->AddObject(UI, CObjectFactory<CUI>::CreateObject());

	//CObjectMgr::GetInstance()->AddObject(UI, CObjectFactory<CBossUI>::CreateObject());
	CSceneMgr::GetInstance()->Initialize();

}

void CMaingame::Update()
{
	
	CKeyMgr::GetInstance()->Update();
	CSceneMgr::GetInstance()->Update();
	switch (CSceneMgr::GetInstance()->GetGameState())
	{
	case TITLE:
		break;
	case SAVE_SELECT:
		break;
	case GAME_SELECT:
		break;
	case MAIN_GAME:
		CObjectMgr::GetInstance()->Update();
		break;
	case EVENT_SCENE:
		break;
	case OPEN_OPTION:
		break;
	default:
		break;
	}
}

void CMaingame::Render()
{
	HDC  hMemDC;
	HBITMAP hBackBit, oldBitmap;
	RECT rt;
	rt.top = 0; rt.left = 0; rt.right = WINCX; rt.bottom = WINCY;

	hMemDC = CreateCompatibleDC(m_hDC);
	hBackBit = CreateCompatibleBitmap(m_hDC, rt.right, rt.bottom);
	oldBitmap = (HBITMAP)SelectObject(hMemDC, hBackBit);
	FillRect(hMemDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));

	switch (CSceneMgr::GetInstance()->GetGameState())
	{
	case TITLE:
	case SAVE_SELECT:
	case GAME_SELECT:
		CSceneMgr::GetInstance()->Render(hMemDC);
		break;
	case MAIN_GAME:
		CSceneMgr::GetInstance()->Render(hMemDC);
		CObjectMgr::GetInstance()->Render(
			dynamic_cast<CPlayer*>(CObjectMgr::GetInstance()->GetPlayer())->GetNowRoomInfo(),hMemDC);
		break;
	case EVENT_SCENE:
		CSoundMgr::GetInstance()->StopAll();

		break;
	case OPEN_OPTION:
		break;
	default:
		break;
	}
	BitBlt(m_hDC, 0, 0, rt.right, rt.bottom, hMemDC, 0, 0, SRCCOPY);
	
	DeleteObject(SelectObject(hMemDC, oldBitmap));
	DeleteDC(hMemDC);
}

void CMaingame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);		
	CObjectMgr::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CImageMgr::GetInstance()->DestroyInstance();
	CBmpMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	
	CSoundMgr::GetInstance()->DestroyInstance();

}
