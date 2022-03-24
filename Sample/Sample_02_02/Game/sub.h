#pragma once
#include <Windows.h>

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND InitWindow(HINSTANCE hInstance, int nCmdShow);