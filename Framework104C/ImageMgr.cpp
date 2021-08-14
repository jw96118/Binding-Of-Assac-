#include "stdafx.h"
#include "ImageMgr.h"
#include "MyImage.h"
IMPLEMENT_SINGLETON(CImageMgr)
CImageMgr::CImageMgr()
{
}


CImageMgr::~CImageMgr()
{
	Release();
}

void CImageMgr::Release()
{

	for (auto& MyPair : imageList)
		SafeDelete(MyPair.second);
	imageList.clear();

}

void CImageMgr::AddImage(const wstring& ImageKey, const WCHAR* wstrFilePath , int ImageEX, int ImageEY)
{
	auto iter_find = imageList.find(ImageKey);

	if (imageList.end() != iter_find)
		return ;

	CMyImage* myImage = new CMyImage;

	if (!myImage->LoadMyImage(wstrFilePath, ImageEX,  ImageEY))
	{
		SafeDelete(myImage);
		//MessageBox(nullptr, wstrFilePath.c_str(), L"Image Load Failed", MB_OK);
		return;
	}
	imageList.insert(make_pair(ImageKey, myImage));
	
	//imageList.insert(make_pair(L"TESTFLOOR", Image::FromFile(L"Image/back/02_lcellarfloor.png")));
}


void CImageMgr::AddImage()
{
	auto iter_find = imageList.find(L"ROOM_RENDER");

	if (imageList.end() != iter_find)
		return;

	CMyImage* myImage = new CMyImage;

	if (!myImage->LoadRoom())
	{
		SafeDelete(myImage);
		//MessageBox(nullptr, wstrFilePath.c_str(), L"Image Load Failed", MB_OK);
		return;
	}
	imageList.insert(make_pair(L"ROOM_RENDER", myImage));

	//imageList.insert(make_pair(L"TESTFLOOR", Image::FromFile(L"Image/back/02_lcellarfloor.png")));
}

HDC CImageMgr::GetDc(const wstring& ImageKey) const
{
	auto iter_find = imageList.find(ImageKey);

	if (imageList.end() == iter_find)
		return nullptr;

	return iter_find->second->GetMemDC();
}



void CImageMgr::AddImage(const wstring& ImageKey, const WCHAR* wstrFilePath)
{
	auto iter_find = imageListGdiP.find(ImageKey);

	if (imageListGdiP.end() != iter_find)
		return;

	imageListGdiP.insert(make_pair(ImageKey, Image::FromFile(wstrFilePath)));

}

Image* CImageMgr::GetImage(const wstring& ImageKey) const
{
	auto iter_find = imageListGdiP.find(ImageKey);

	if (imageListGdiP.end() == iter_find)
		return nullptr;

	return iter_find->second;
}
