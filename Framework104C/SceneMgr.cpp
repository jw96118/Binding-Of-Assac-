#include "stdafx.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Title.h"
#include "SaveSelect.h"
#include "GameSelect.h"
#include "Stage.h"
#include "Room.h"
IMPLEMENT_SINGLETON(CSceneMgr)
CSceneMgr::CSceneMgr()
{
}


CSceneMgr::~CSceneMgr()
{
	Relese();
}

void CSceneMgr::Initialize()
{
	gameState = TITLE;
	preGameState = TITLE;
	mainX = 0;
	mainY = 0;
	scrollCount = 0;
	scrollFlag = false;
	sceneVec.push_back(new CTitle);
	sceneVec.push_back(new CSaveSelect);
	sceneVec.push_back(new CGameSelect);
	sceneVec.push_back(new CStage);
	for (unsigned int i = 0; i < sceneVec.size(); i++)
	{
		sceneVec[i]->Initialize();
	}

	//CSoundMgr::GetInstance()->PlayBGM(L"title screen.ogg");

}

void CSceneMgr::Update()
{
	KeyInput();
	for (unsigned int i = 0; i < sceneVec.size(); i++)
	{
		sceneVec[i]->SetRenderPt(mainX, mainY);
	}
	switch (gameState)
	{
	case TITLE:
		sceneVec[TITLE]->Update();
		ScrollUp(GAME_SELECT, 0);
		PlayerBGM(L"title screen.ogg");
		break;
	case SAVE_SELECT:
		sceneVec[SAVE_SELECT]->Update();
		//ScrollDown(TITLE, -WINCY);
		//ScrollUp(GAME_SELECT, -WINCY);
		break;
	case GAME_SELECT:
		sceneVec[GAME_SELECT]->Update();
		ScrollDown(TITLE, -WINCY );

		break;
	case MAIN_GAME:
		sceneVec[MAIN_GAME]->Update();
		if (endingflag)
		{
			gameState = EVENT_SCENE;
		}
		break;
	case EVENT_SCENE:
		break;
	case OPEN_OPTION:
		break;
	default:
		break;
	}
}

void CSceneMgr::Render(HDC hDC)
{
	Graphics g(hDC);
	switch (gameState)
	{
	case TITLE:
	case SAVE_SELECT:
	case GAME_SELECT:
		for (int i = 0; i < 3; i++)
		{
			if (i != 1)
				sceneVec[i]->Render(hDC);
		}
		break;
	case MAIN_GAME:
		sceneVec[MAIN_GAME]->Render(hDC);
		break;
	case EVENT_SCENE:
		break;
	case OPEN_OPTION:
		break;
	default:
		break;
	}
}

GAME_STATE CSceneMgr::GetGameState()
{
	return gameState;
}

void CSceneMgr::Relese()
{
	for_each(sceneVec.begin(), sceneVec.end(), SafeDelete<CScene*>);
	sceneVec.clear();
}

void CSceneMgr::KeyInput()
{
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_A) || CKeyMgr::GetInstance()->KeyPressing(KEY_S) || CKeyMgr::GetInstance()->KeyPressing(KEY_D) || CKeyMgr::GetInstance()->KeyPressing(KEY_W))
	{
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_A))
		{
		}
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_D))
		{
		}
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_W))
		{
		}
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_S))
		{
		}
	}

	if (CKeyMgr::GetInstance()->KeyPressing(KEY_RIGHT))
	{
	}
	else if (CKeyMgr::GetInstance()->KeyPressing(KEY_LEFT))
	{
	}
	else if (CKeyMgr::GetInstance()->KeyPressing(KEY_UP))
	{

	}
	else if (CKeyMgr::GetInstance()->KeyPressing(KEY_DOWN))
	{

	}

	if (CKeyMgr::GetInstance()->KeyDown(KEY_SPACE) || CKeyMgr::GetInstance()->KeyDown(KEY_RETURN))
	{

		if (!scrollFlag)
		{

			scrollFlag = true;
			scrollCount = 38;
			switch (gameState)
			{
			case TITLE:
				gameState = GAME_SELECT;
				preGameState = TITLE;
				CSoundMgr::GetInstance()->PlaySound(L"book page turn.wav", SOUNDMGR::SCENE);

				break;
			case SAVE_SELECT:
			/*	gameState = GAME_SELECT;
				preGameState = SAVE_SELECT;
				CSoundMgr::GetInstance()->PlaySound(L"book page turn.wav", SOUNDMGR::SCENE);
				*/break;
			case GAME_SELECT:
				if (dynamic_cast<CGameSelect*>(sceneVec[GAME_SELECT])->GetSelectState() == NEW_RUN)
				{
					StopBGM(SOUNDMGR::BGM);
					gameState = MAIN_GAME;
					preGameState = GAME_SELECT;
					mainX = 0;
					mainY = 0;
				}
				break;
			case MAIN_GAME:
				//gameState = GAME_SELECT;
				break;
			case EVENT_SCENE:
				break;
			case OPEN_OPTION:
				//gameState = MAIN_GAME;
				break;
			}
		}
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_ESCAPE))
	{
		if (!scrollFlag)
		{
			CSoundMgr::GetInstance()->PlaySound(L"book page turn.wav", SOUNDMGR::SCENE);

			scrollFlag = true;
			scrollCount = 38;
			switch (gameState)
			{
			case TITLE:
				PostQuitMessage(0);
				return;
			case SAVE_SELECT:
				/*gameState = TITLE;
				preGameState = SAVE_SELECT;*/
				break;
			case GAME_SELECT:
				gameState = TITLE;
				preGameState = GAME_SELECT;
				break;
			case MAIN_GAME:
				gameState = OPEN_OPTION;
				break;
			case EVENT_SCENE:
				break;
			case OPEN_OPTION:
				gameState = MAIN_GAME;
				break;
			}
		}
	}
}

void CSceneMgr::ScrollDown(GAME_STATE _state, int _y)
{
	if (preGameState == _state)
	{
		if (mainY > _y)
		{
			mainY -= scrollCount;
			if (scrollCount > 1)
			{
				scrollCount--;
			}
		}
		else
		{
			mainY = _y;
			scrollFlag = false;
		}
	}
}
void CSceneMgr::ScrollUp(GAME_STATE _state, int _y)
{
	if (preGameState == _state)
	{
		if (mainY < _y)
		{
			mainY += scrollCount;
			if (scrollCount > 1)
			{
				scrollCount--;
			}
		}
		else
		{
			mainY = _y;
			scrollFlag = false;
		}
	}
}

void CSceneMgr::PlayerBGM(const wstring & wstrSoundKey)
{
	if (!soundPlay)
	{
		CSoundMgr::GetInstance()->PlayBGM(wstrSoundKey);
		soundPlay = true;
	}
}

void CSceneMgr::StopBGM(SOUNDMGR::CHANNEL_ID eID)
{
	CSoundMgr::GetInstance()->StopSound(eID);
	soundPlay = false;
}

vector<CRoom*>& CSceneMgr::GetRoomInfo()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return dynamic_cast<CStage*>(sceneVec[MAIN_GAME])->GetRoomInfo();
}

CScene* CSceneMgr::GetGrid() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return sceneVec[MAIN_GAME];
}

const bool & CSceneMgr::GetCutScene()
{
	return dynamic_cast<CStage*>(sceneVec[MAIN_GAME])->GetCutScene();
}

void CSceneMgr::SetMapScroll(int i)
{
	dynamic_cast<CStage*>(sceneVec[MAIN_GAME])->SetScroll(i);
}

void CSceneMgr::SetCutScene(bool flag)
{
	dynamic_cast<CStage*>(sceneVec[MAIN_GAME])->SetCutScene(flag);

}


