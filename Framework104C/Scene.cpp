#include "stdafx.h"
#include "Scene.h"


CScene::CScene()
{
}


CScene::~CScene()
{
}

void CScene::Initialize()
{
}

int CScene::Update()
{
	return 0;
}

void CScene::Render(HDC hDC)
{
}

void CScene::Release()
{
}

void CScene::PlayerBGM(const wstring & wstrSoundKey)
{
	if (!soundPlay)
	{
		CSoundMgr::GetInstance()->PlayBGM(wstrSoundKey);
		soundPlay = true;
	}
}

void CScene::PlayerSound(const wstring & wstrSoundKey, SOUNDMGR::CHANNEL_ID eID)
{
	if (!soundPlay)
	{
		CSoundMgr::GetInstance()->PlaySound(wstrSoundKey,eID);
		soundPlay = true;
	}
}

void CScene::StopBGM(SOUNDMGR::CHANNEL_ID eID)
{
	CSoundMgr::GetInstance()->StopSound(eID);
	soundPlay = false;
}

void CScene::SetRenderPt(int x, int y)
{
	renderX = (float)x;
	renderY = (float)y;

}
