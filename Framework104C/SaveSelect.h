#pragma once
#include "Scene.h"
class CSaveSelect :
	public CScene
{
public:
	CSaveSelect();
	~CSaveSelect();

	// CScene��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC hDC) override;
private:
	virtual void Release() override;
};

