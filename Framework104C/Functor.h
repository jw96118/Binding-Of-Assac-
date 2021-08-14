#pragma once

#ifndef __FUNCTOR_H__

#include "GameObject.h"

class CSortObjectY
{
public:
	bool operator()(CGameObject* pDst, CGameObject* pSrc)
	{
		return pDst->GetInfo().fY < pSrc->GetInfo().fY;
	}
};

#define __FUNCTOR_H__
#endif
