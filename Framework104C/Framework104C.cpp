// Framework104C.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Framework104C.h"
#include "Maingame.h"
#include <Vfw.h>

#pragma comment(lib, "vfw32.lib")
#define MAX_LOADSTRING 100
#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
// 전역 변수:
HWND g_hWnd;
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND endingScene;
bool testflag = false;
bool endingflag = false;
// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FRAMEWORK104C, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FRAMEWORK104C));

    MSG msg;	
	msg.message = WM_NULL;

	CMaingame	mainGame;
	mainGame.Initialize();

	// GetTickCount: 운영체제가 시작된 후부터 흐른 시간을 1000분의 1초 단위로 카운팅하는 함수.
	DWORD dwOldTime = GetTickCount();
	DWORD dwCurTime = 0;
	
    // 기본 메시지 루프입니다.
	while (WM_QUIT != msg.message)
	{	
		
		// PM_REMOVE: 메시지 큐에서 먼저 들어온 메시지를 제거함.
		// PM_NOREMOVE: 메시지 큐에서 메시지를 제거하지 않고 그대로 둠.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		dwCurTime = GetTickCount();

		if (dwCurTime - dwOldTime >= 10)
		{
			if (CSceneMgr::GetInstance()->GetGameState() != EVENT_SCENE)
			{
				mainGame.Update();
				mainGame.Render();
			}
			dwOldTime = dwCurTime;
		}	
		if (CSceneMgr::GetInstance()->GetGameState() == EVENT_SCENE)
		{
			if (!testflag)
			{
				endingScene = MCIWndCreate(g_hWnd, hInst, MCIWNDF_NOTIFYMODE | MCIWNDF_NOPLAYBAR |
					WS_VISIBLE | WS_CHILD, L"017_defeating_mega_satan.wmv");
				MoveWindow(endingScene, 0, 0, WINCX, WINCY, NULL);

				MCIWndPlayTo(endingScene, 38000);


				testflag = true;
			}
		}
	}	

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FRAMEWORK104C));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	// HINSTANCE: 현재 프로그램의 핸들
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT rc = { 0, 0, WINCX, WINCY };

   // 순수 클라이언트 영역의 크기를 조정하는 함수.
   // 첫번째 인자로 사용자가 원하는 크기의 RECT를 전달(입력)하면 그 크기만큼 클라이언트 영역에 반영이 되고
   // 클라이언트 영역 크기 + 테두리 영역 크기를 다시 첫번째 인자로 기록(출력)한다.
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   // H (Handle): 인스턴스를 식별하고 제어하기 위한 정수 값. 
   // 시스템이 부여하고 중복 값을 가질 수 없다.
   // HWND: 생성된 창(윈도우)의 핸들.
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//CallBack 함수: 사용자가 아니라 시스템이 호출하는 함수.

	// 함수 호출 규약
	// 인자 전달 순서는 어떻게 하며, 함수 종료될 때 스택 정리를 누가 할 것인가?
	// __cdecl:		인자 전달 순서 오른쪽 -> 왼쪽 진행. 호출자가 stack 정리.
	// __stdcall:	인자 전달 순서 오른쪽 -> 왼쪽 진행. 피호출자가 stack 정리.
	// __fastcall:	인자 전달 순서 오른쪽 -> 왼쪽 진행. 피호출자가 stack 정리.
	// __thiscall:	인자 전달 순서 오른쪽 -> 왼쪽 진행. 피호출자가 stack 정리.

    switch (message)
    {      
	case MCIWNDM_NOTIFYMODE:
	{
		LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
		
		switch (lParam)
		{
		case MCI_MODE_STOP:
			if (endingflag)
			{
				MCIWndEnd(endingScene);
				endingflag = false;
			}
			PostQuitMessage(0);
			break;
		}
		break;
	}
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}