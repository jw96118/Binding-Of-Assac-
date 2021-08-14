// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once
#pragma comment(lib, "gdiplus.lib")
#include "targetver.h"

//#include "vld.h"
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

// Standard Headers

#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <ctime>
#include <iostream>
#include <ole2.h>
#include <gdiplus.h>
using namespace std;
using namespace Gdiplus;
// User Headers
#include "Define.h"
#include "Enum.h"
#include "Extern.h"
#include "Struct.h"
#include "Function.h"
#include "Typedef.h"
#include "Functor.h"
// Managers 

#include "SoundMgr.h"
#include "ObjectFactory.h"
#include "CollisionMgr.h"
#include "ObjectMgr.h"
#include "KeyMgr.h"
#include "ImageMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"


