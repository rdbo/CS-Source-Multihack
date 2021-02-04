#pragma once
#ifndef WINDOWDATA_H
#define WINDOWDATA_H

#include <pch.h>

class WindowData
{
private:
	RECT wRect   = {};
	HWND hWindow = (HWND)NULL;

public:
	WindowData(HWND hWnd);
	~WindowData();

public:
	HWND GetHandle();
	RECT GetWindowRect();
	LONG GetWidth();
	LONG GetHeight();
};

#endif