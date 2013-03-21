#include "StdAfx.h"
#include "warmgui.h"
#include "Window.h"
#include "WindowManager.h"

using namespace WARMGUI;

CWindowManager::CWindowManager(void)
{
}

CWindowManager::~CWindowManager(void)
{
	clear();
}

bool CWindowManager::Add(CWindow* pwnd)
{
	if (!pwnd) return false;

	IterBool ib = insert(WindowPair(pwnd->GetSafeHwnd(), pwnd));
	return ib.second;
}

bool CWindowManager::Remove(CWindow * pwnd)
{
	if (!pwnd) return false;

	return Remove(pwnd->GetSafeHwnd());
}


bool CWindowManager::Remove(HWND hwnd)
{
	if (hwnd) return false;

	WndIterator wi = find(hwnd);

	if (wi == end( )) {
		return false;
	} else {
		erase(wi);
		return true;
	}
}

CWindow * CWindowManager::Find(HWND hwnd) const
{
	ConstWndIterator wi = find(hwnd);

	if (wi == end( )) {
		return (0);
	} else {
		return wi->second;
	}
}


void CWindowManager::Clear()
{
	clear();
}


CWindowManager * CWindowManager::GetInstance()
{
	static CWindowManager wm;
	return &wm;
}




