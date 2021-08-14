#pragma once

#ifndef __STRUCT_H__

typedef struct tagInfo
{
	float fX, fY;
	float fCX, fCY;
}INFO;
typedef struct floorCol
{
	RECT COL_1 = {};
	RECT COL_2 = {};
}FCol;
typedef struct tagStatus
{
	float moveSpeed;
	float shotSpeed;
	float luck;
	float atk;
	float bulletSpeed;
	float hp;
	float maxHp;
	float range;
}STATUS;

#define __STRUCT_H__
#endif
