#include <pch.h>
#include "windowdata.h"

WindowData::WindowData(HWND hWnd)
{
	this->hWindow = hWnd;
}

WindowData::~WindowData()
{
	this->hWindow = (HWND)NULL;
	this->wRect   = {};
}

HWND WindowData::GetHandle()
{
	return this->hWindow;
}

RECT WindowData::GetWindowRect()
{
	GetClientRect(this->hWindow, &this->wRect);
	return this->wRect;
}

LONG WindowData::GetWidth()
{
	this->GetWindowRect();
	return this->wRect.right - this->wRect.left;
}

LONG WindowData::GetHeight()
{
	this->GetWindowRect();
	return this->wRect.bottom - this->wRect.top;
}
