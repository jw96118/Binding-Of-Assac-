#pragma once

#ifndef __DEFINE_H__

#define WINCX 1280
#define WINCY 768

#define NO_EVENT 0
#define DEAD_OBJ 1

#define DOOR_SIZE 30

#define WALL_X_SIZE 140
#define WALL_Y_SIZE 130

#define SCROLL_SPEED_X 70
#define SCROLL_SPEED_Y 50

#define PI 3.141592f
#define GRAVITY 9.8f
//플레이어 애니메이션
#define SIZEX 32
#define SIZEY 32
//바닥 이미지
#define FLOORSIZEX 233
#define FLOORSIZEY 154

#define DEADIMAGEX 64
#define DEADIMAGEY 64

#define EXPLOSIONIMAGEX 96
#define EXPLOSIONIMAGEY 96

#define MONSTROIMAGEX 80
#define MONSTROIMAGEY 112

#define SHADOWX 120
#define SHADOWY 49

#define NULL_CHECK(ptr) if(nullptr == (ptr)) return;
#define NULL_CHECK_RETURN(ptr, val) if(nullptr == (ptr)) return val;

#define DECLARE_SINGLETON(ClassName)	\
public:									\
	static ClassName* GetInstance();	\
	void DestroyInstance();				\
private:								\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)			\
ClassName* ClassName::m_pInstance = nullptr;	\
ClassName* ClassName::GetInstance()				\
{												\
	if(nullptr == m_pInstance)					\
		m_pInstance = new ClassName;			\
	return m_pInstance;							\
}												\
void ClassName::DestroyInstance()				\
{												\
	if(m_pInstance)								\
	{											\
		delete m_pInstance;						\
		m_pInstance = nullptr;					\
	}											\
}

#define __DEFINE_H__
#endif